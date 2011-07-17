using namespace std;
#include <string>
#include <set>
#include <iostream>

#include <bigmc.h>

node::node() {
	parent = NULL;
	ctrl = NULL;
	arity = 0;
	active = false;
}

node::node(control c) {
	parent = NULL;
	ctrl = c;
	arity = bigraph::arity(c);
	active = bigraph::activity(c);

	if(arity > 0)
		port = vector<name>(arity,0);
}

node::~node() {
	// TODO should recursively delete children
}

void node::add(node *n) {
	children.insert(n);
	n->set_parent(this);
}

node *node::get_parent() {
	return parent;
}

set<node *> node::get_children() {
	return children;
}

int node::get_arity() {
	return arity;
}

void node::set_port(int i, name n) {
	port[i] = n;
}

name node::get_port(int i) {
	return port[i];
}

void node::set_parent(node *n) {
	parent = n;
}

string node::to_string() {
	string nm = "";

	if(ctrl == 0) {
		nm = "Region_0";
	} else {
		nm = bigraph::control_to_string(ctrl) + " : " + (active ? "act" : "pas");
	}

	string s = nm;

	string prt = "";

	if(port.size() > 0) {
		for(unsigned int i = 0; i<port.size(); i++) {
			if(port[i])
				prt += "[" + bigraph::name_to_string(port[i]) + "]";
			else
				prt += "[.]";
		}
	}

	set<node *> ch = get_children();
	if(get_children().size() == 0) return s + prt;


	
	s += prt + " { ";

	set<node *>::iterator it = ch.begin();
	while(it != ch.end()) {
		s += (*it)->to_string() + ", ";
		++it;
	}

	return s + "}";
}

bool node::is_hole() {
	return false;
}

// Here's how matching works:
/* We first construct a map from controls to lists of nodes in the model, cmap
 * We construct a map of candidate matches, cand, from nodes to match objects
 * We also construct a vector of match roots, roots
 * Then we start traversing the redex.
 * For a given node n, we look up all possible places it could match in cmap[n->ctrl]
 * We exclude passive contexts at this point, and end up with a set of potential matches
 * We then check if any of these are children of nodes in cand.
 * If a match is a child, the candidate match object is updated, and the node is added to the
 * new candidate map, cand2.
 * When the redex work queue is finished (i.e. traversal is done), the match objects in
 * cand2 are the set of matches.
 */

vector<node *> node::flatten(node *n) {
	vector<node *> v;

	set<node *> ch = n->get_children();

	v.insert(v.end(),ch.begin(),ch.end());

	for(set<node *>::iterator i = ch.begin(); i != ch.end(); ++i) {
		vector<node *> cc = flatten(*i);
		v.insert(v.end(),cc.begin(),cc.end());
	}

	return v;
}

set<match *> node::find_matches(node *redex) {
	// Construct the control map
	map<control,vector<node *> > cmap;
	vector<node *> tm = flatten(this);
	cmap[ctrl] = vector<node *>();
	cmap[ctrl].push_back(this);

	for(unsigned int i = 0; i<tm.size(); i++) {
		if(tm[i]->is_hole()) continue;
		cmap[tm[i]->ctrl].push_back(tm[i]);
		cout << "Adding to control map: " << tm[i]->to_string() << endl;
	}

	// Define the candidate map
	map<node *,match *> cand;
	map<node *,match *> cand2;
	cand2[this] = new match(this);
	
	// Redex work queue
	vector<node *> work = flatten(redex);

	unsigned int operations = 0;
	
	for(unsigned int i = 0; i<work.size(); i++) {
		cout << " ****** Cand: " << cand.size() << endl;
		cout << " ****** Cand2: " << cand2.size() << endl;
		// FIXME: We're leaking match objects here.  Need to deallocate unused matches.
		
		cand.swap(cand2);
		cand2 = map<node *,match *>();

		cout << " ****** Cand': " << cand.size() << endl;
		cout << " ****** Cand2': " << cand2.size() << endl;

		if(work[i]->is_hole()) {
			// FIXME: hole matching is greedy - it will take everything or nothing
			for(map<node *,match *>::iterator h = cand.begin(); h!=cand.end(); ++h) {
				set<node *> ch = h->first->get_children();
				node *fr = new node();
				for(set<node *>::iterator j = ch.begin(); j!=ch.end(); ++j)
					fr->add(*j);

				cout << "BUG: work->is_hole(): " << h->first << ": " << h->second << " adding: " << fr << endl;
				if(h->second == NULL) {
					continue;
					//h->second = new match(h->first);
					//h->second->add_match(work[i],h->first);
					//exit(1);
				}

				h->second->add_param(((hole *)work[i])->index, fr);
				cand2[h->first] = h->second;
			}

			continue;
		}

		cout << "*** Work queue traversal: " << i << ": " << work[i]->to_string() << endl;
		vector<node *> cn = cmap[work[i]->ctrl];
		if(cn.size() == 0) {
			cout << "  NO MATCH" << endl;
			// todo: break here?
			continue;
		}

		operations++;

		for(unsigned int j = 0; j<cn.size(); j++) {
			cout << " * Candidate: " << cn[j]->to_string();
			// For the root of a match, we need to only match in active contexts
			if(i == 0 && cn[j]->parent != NULL && !cn[j]->parent->is_active_context()) {
				cout << " ** Candidate is not active context" << endl;
				continue;
			}

			// At the root of the match, so we need to seed the candidate map
			if(i == 0) {
				cand2[cn[j]] = new match(cn[j]);
				cand2[cn[j]]->add_match(work[i],cn[j]);
				cout << " ** Adding candidate to root work queue: " << cand2[cn[j]] << endl;
			} else {
				if(cn[j]->get_parent() != NULL && cand[cn[j]->get_parent()] != NULL) {
					// We have a live one!  This is a real partial match.
					cand2[cn[j]] = cand[cn[j]->get_parent()];
					cand2[cn[j]]->add_match(work[i],cn[j]);
					cout << "!!!!!!  Partial Match: " << cn[j]->to_string() << endl;
				} else {
					cout << "  Not a match: Parent: ";
					if(cn[j]->parent == NULL) {
						cout << "NULL cand: ";
					} else {
						cout << "non-null" << endl;
						cout << " Parent: " << cn[j]->get_parent();
					}

					cout << " candidate2: " << cand2[cn[j]] << endl;

					for(map<node *,match *>::iterator k = cand.begin(); k!= cand.end(); ++k) {
						cout << " Candidate dump: " << k->first << " equality check: " << (k->first == cn[j]) << " value: " << k->second << endl;
					}
				}
			}

		}



	}

	cout << "BUG: Redex match efficiency: " << operations << " / " << work.size() << endl;

	set<match *> res;
	for(map<node*,match*>::iterator j = cand2.begin(); j!=cand2.end(); ++j) {
		res.insert(j->second);
	}

	return res;
}

bool node::matches(node *other) {
	// matches should always be called on the redex!
	// as in, redexnode.matches(modelnode)

	return ctrl == other->ctrl;
}

bool node::is_active_context() {
	// The root is always "active", even if nothing else is
	// TODO: OPT: cache this result

	//cout << "node::is_active_context(): " << to_string() << endl;

	if(!active && ctrl != 0) {
		cout << "node::is_active_context(): false" << endl;
		return false;
	}

	if(ctrl == 0 || parent == NULL) {
		cout << "node::is_active_context(): true" << endl;
		return true;
	}

	bool pactive = parent->is_active_context();
	//cout << "node::is_active_context(): active: " << active << " pactive: " << pactive << endl;

	return active && pactive;
}
