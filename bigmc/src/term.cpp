using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <bigmc.h>

term::term() {
	type = 0;
	remaining.push_back(this);
	parent = NULL;
}

term::~term() {
}

string term::to_string() {
	return "<untyped term>";
}

set<match *> term::find_matches(match *m) {
	assert(0);
	switch(type) {
		case TPREF: {
			cout << "term::find_matches(): prefix\n";
			break;
		} 
		case TPAR:
			cout << "term::find_matches(): par\n";
		break;
		case THOLE:
			cout << "term::find_matches(): hole\n";

		break;
		case TNIL:
			cout << "term::find_matches(): nil\n";
		break;
		default:
			cerr << "term::find_matches(): Unknown node type " << type << endl;
			exit(1);
		break;
	}

	return set<match*>();
}

term *term::apply_match(match *m) {
	assert(0);
	return this;
}

term *term::instantiate(match *m) {
	assert(0);
	return NULL;
}

unsigned int term::size() {
	return 0;
}

void term::accept(termvisitor *t) {
	t->visit(this);
}

bool term::active_context() {
	if(parent == NULL) return true;

	return parent->active_context();
}

parallel::parallel(set<term *>l) {
	terms = l;
	type = TPAR;

	for(set<term *>::iterator i = l.begin(); i!=l.end(); i++) {
		(*i)->parent = this;
	}
}

parallel::~parallel() {
	terms.clear();
}

string parallel::to_string() {
	string s = "(";

	unsigned int k = 0;
	for(set<term *>::iterator i = terms.begin(); i!=terms.end(); ++i) {
		k++;

		s += (*i)->to_string();

		if(k < terms.size())
			s += " | ";
	}

	return s + ")";
}

set<match *> parallel::find_matches(match *m) {
	if(DEBUG) cout << "BUG: parallel::find_matches()" << endl;

	if(m->has_succeeded || m->has_failed) return set<match *>();

	list<term *> r = m->remaining();


	if(r.size() == 0) {
		if(DEBUG) cout << "BUG: parallel::find_matches(): SUCCESS (nothing to match)" << endl;
		m->success();
		return match::singleton(m);
	}

	if(r.size() == 1 && r.front()->type == TPAR) {
		if(DEBUG) cout << "BUG: parallel::find_matches(): generating permutation" << endl;
		set<term *> c = ((parallel *)r.front())->get_children();
		list<term *> cl;
		set<match *> matches;

		if(m->root == NULL && !active_context())
			return m->failure();

		if(m->root == NULL) {
			if(DEBUG) cout << "BUG: Null root in match, setting to TPAR: " << to_string() << endl;
			m->root = this;
		}

		for(set<term*>::iterator i = c.begin(); i!=c.end(); i++)
			cl.push_back(*i);

		do {
			if(DEBUG) {
				cout << "BUG: Permutation: result set: " << matches.size() << endl;
				for(list<term *>::iterator i = cl.begin(); i!=cl.end(); i++) {
					cout << " * " << (*i)->to_string() << endl;
				}
			}

			match *mp = m->clone(NULL, cl);
			set<match *> res = find_matches(mp);

			matches = match::merge(matches,res);
		} while(next_permutation(cl.begin(),cl.end()));

		return matches;
	}

	if(r.size() > 1) {
		/* So we have a situation like this:
			The current node is a parallel term like this:
			( A | B | C )
			We have a permutation like this:
			[ A, B, C ]
			So we iterate over the current children (A | B | C) in-order
			and try to find matches for A and then for C.
			Once a match is "consumed", we remove it from the redex permutation
			and proceed to the next next.  There is no back-tracking or anything
			necessary.

			We can't just match directly, to avoid spoiling the
			match object.  Instead we clone it, and on a successful match,
			we incorporate the match.
		*/

		if(DEBUG) { cout << "BUG: Ready to match permutation against: " << to_string() << " Permutation: ";
		for(list<term *>::iterator i = r.begin(); i!=r.end(); i++) {
			cout << (*i)->to_string() << ", ";
		}
		cout << endl;
		}

		set<match *> results;

		for(set<term *>::iterator i = terms.begin(); i!=terms.end(); i++) {
			if(DEBUG) cout << "BUG: parallel::find_matches(): iterating child: " << (*i)->to_string() << endl;
			
			if(r.size() == 0) {
				if(DEBUG) cout << "BUG: parallel::find_matches(): SUCCESS" << endl;
				// We have succeeded!  We're done!
				m->success();
				results.insert(m);
				return results;
			}

			if(r.size() == 1 && r.front()->type == THOLE) {
				// This is success.  A hole as the last
				// thing to match can match everything that is left.
				m->success();
				int idx = ((hole *)r.front())->index;
				parallel *holecontents = new parallel(set<term *>());

				while(i != terms.end()) {
					holecontents->terms.insert(*i);
					m->add_match(*i,r.front());
					i++;
				}

				if(holecontents->terms.size() == 0) {
					delete holecontents;
					m->add_param(idx,new nil());
				}

				else if(holecontents->terms.size() == 1) {
					m->add_param(idx,*(holecontents->terms.begin()));
					delete holecontents;
				}

				else {
					m->add_param(idx,holecontents);
				}

				results.insert(m);
				r.pop_front();
				return results;
			}

			term *nm = r.front();

			if(DEBUG) cout << "BUG: parallel::find_matches(): matching " << (*i)->to_string() << " against " << nm->to_string() << endl;
			// Clone the match and attempt to match
			match *cl = m->clone(NULL, term::singleton(nm));

			if(DEBUG) cout << "cl: " << cl->to_string() << endl;

			set<match *> res = (*i)->find_matches(cl);

			if(cl->has_failed || !cl->has_succeeded) {
				if(DEBUG) cout << "BUG: parallel::find_matches(): match failed" << endl;
				delete cl;
				// Discard the matches we found
				// FIXME leaking matches in the set res here.
				return m->failure();
				//continue;
			} else if (cl->has_succeeded) { // Success!
				if(DEBUG) cout << "BUG: parallel::find_matches(): match succeeded" << endl;
				
				// FIXME: leaking matches in the set res here.

				m->advance(NULL, term::singleton(nm));
				m->incorporate(cl);
				match::merge(results,res);
				r.pop_front();
				delete cl;
			} else {
				if(DEBUG) cout << "BUG: parallel::find_matches(): match ... died?" << endl;
				// FIXME: Um, what does this condition mean?
				delete cl;
				continue;
			} 

			if(DEBUG) cout << "BUG: parallel::find_matches(): match obj: " << m->to_string() << endl;
		}

		if(r.size() == 0) {
			m->success();
			results.insert(m);
			if(DEBUG) cout << "BUG: parallel::find_matches(): match succeeded: " << m->to_string() << " Result Size: " << results.size() << endl;
			return results;
		} else {
			if(DEBUG) cout << "BUG: parallel::find_matches(): match failed: " << m->to_string() << " r.size(): " << r.size() << endl;
			if(DEBUG) cout << "BUG: r[0]: " << r.front()->to_string() << endl;
			return m->failure();
		}
	}
	

	return m->failure();
} 

set<term *> parallel::get_children() {
	return terms;
}

term *parallel::apply_match(match *m) {
	if(this == m->root) {
		// This is the match site.  Return the reactum.
		if(DEBUG) cout << "BUG: parallel::apply_match(): Found match site!" << endl;
		term *r = m->get_rule()->reactum->instantiate(m);
		if(DEBUG) cout << "Returning reactum: " << r->to_string() << endl;
		return r;
	} 

	set<term *> n;

	for(set<term*>::iterator i = terms.begin(); i!=terms.end(); i++) {
		n.insert((*i)->apply_match(m));
	}

	return new parallel(n);
}

term *parallel::instantiate(match *m) {
	set<term *> n;

	for(set<term*>::iterator i = terms.begin(); i!=terms.end(); i++) {
		if((*i)->type != TPAR)
			n.insert((*i)->instantiate(m));
		else {
			parallel *p = (parallel *)(*i);
			n.insert(p->terms.begin(), p->terms.end());
		}
	}

	return new parallel(n);
}

unsigned int parallel::size() {
	unsigned int sz = 0;
	for(set<term *>::iterator i = terms.begin(); i!=terms.end(); i++) {
		sz += (*i)->size();
	}

	return sz;
}

void parallel::accept(termvisitor *t) {
	for(set<term*>::iterator i = terms.begin(); i != terms.end(); i++) {
		(*i)->accept(t);
	}

	t->visit(this);
}

prefix::prefix(control c, vector<name> ports, term *suff) {
	ctrl = c;
	arity = bigraph::arity(c);
	active = bigraph::activity(c);

	port = ports;
	suffix = suff;

	if(suffix)
		suffix->parent = this;

	type = TPREF;

	if(port.size() > arity) {
		cerr << "Error: control " << bigraph::control_to_string(c) << " has arity " << arity << " but " << port.size() << " ports have been linked!" << endl;
		exit(1);
	}
}

prefix::~prefix() {
	delete suffix;
}

string prefix::to_string() {
	string nm = "";

	if(port.size() == 0)
		nm = "";
	else {
		for(int i = 0; i<port.size(); i++) {
			if(port[i] == 0) {
				nm += "_";
			} else {
				nm += bigraph::name_to_string(port[i]);
			}
			if(i != port.size()-1)
				nm += ",";
		}

		if(nm != "") nm = "[" + nm + "]";
	}
	if(suffix != NULL)
		return bigraph::control_to_string(ctrl) + nm + "." + suffix->to_string();
	else
		return bigraph::control_to_string(ctrl) + nm + ".NULL";
}

term *prefix::get_suffix() {
	return suffix;
}

set<match *> prefix::find_matches(match *m) {
	if(DEBUG) cout << "BUG: prefix::find_matches()" << endl;

	if(m->has_succeeded || m->has_failed) return set<match *>();

	list<term *> r = m->remaining();

	if(r.size() == 0 || (r.size() == 1 && r.front()->type == TNIL)) {
		return m->failure();
	}

	if(r.size() == 1) {
		term *t = r.front();

		if(t->type == TPREF) {
			if(DEBUG) cout << "BUG: prefix::find_matches(): redex front: " << t->to_string() << endl;
			prefix *tp = dynamic_cast<prefix *>(t);
			assert(tp != NULL);

			if(tp->ctrl == ctrl) {
				if(DEBUG) cout << "BUG: prefix::find_matches(): match found" << endl;
				// We have a match!

				if(m->root == NULL && !active_context())
					return m->failure();

				// If this is a new match, set the root to here.
				if(m->root == NULL)
					m->root = this;
				
				m->add_match(t,this);

				// Now we send this match on its merry way into the suffix:
				m->advance(suffix, term::singleton(tp->suffix));
				
				return suffix->find_matches(m);

				// And we need to create a new match that could potentially start
				// here! 

				//match *newmatch = m->fresh(NULL, term::singleton(m->get_rule()->redex));
				
				//return match::merge(suffm,suffix->find_matches(newmatch));
				//return suffm;
			} else {
				if(DEBUG) cout << "BUG: prefix::find_matches(): match not found" << endl;
				//match *newmatch = m->fresh(NULL, term::singleton(m->get_rule()->redex));
				//return suffix->find_matches(newmatch);
				return m->failure();
			}

		} else if(t->type == THOLE) {
			if(m->root == NULL && !active_context())
				return m->failure();

			// A hole will always match a prefix.
			if(m->root == NULL)
				m->root = this;
			m->success();
			m->add_param(((hole *)t)->index, this);
			m->add_match(t,this);
			m->advance(NULL,list<term*>());
			return match::singleton(m);
		} else {
			return m->failure();
		}
	}

	return m->failure();
} 

term *prefix::apply_match(match *m) {
	if(this == m->root) {
		// This is the match site.  Return the reactum.
		if(DEBUG) cout << "BUG: prefix::apply_match(): Found match site!" << endl;
		term *r = m->get_rule()->reactum->instantiate(m);
		if(DEBUG) cout << "Returning reactum: " << r->to_string() << endl;
		return r;
	} else {
		if(DEBUG) cout << "BUG: prefix::apply_match(): not match site: " << this << " != " << m->root << endl;
		return new prefix(ctrl,port,suffix->apply_match(m));
	}
}

term *prefix::instantiate(match *m) {
	set<term *> n;

	return new prefix(ctrl,port,suffix->instantiate(m));
}

unsigned int prefix::size() {
	return 1 + suffix->size();
}

void prefix::accept(termvisitor *t) {
	suffix->accept(t);
	t->visit(this);
}

bool prefix::active_context() {
	if(parent == NULL) return true;

	return active && parent->active_context();
}

hole::hole(int idx) {
	index = idx;
	type = THOLE;
}

hole::~hole() {
}

string hole::to_string() {
	std::string s;
	std::stringstream out;
	out << index;
	return "$" + out.str();
}

set<match *> hole::find_matches(match *m) {
	if(DEBUG) cout << "BUG: hole::find_matches()" << endl;

	m->advance(NULL,list<term*>());
	return set<match *>();
} 

term *hole::apply_match(match *m) {
	// Erm, no.  There shouldn't be any holes in the place graph.
	if(DEBUG) cerr << "ERROR: hole::apply_match(): Invalid place graph contains a hole." << endl;
	exit(1);
	return NULL;
}

term *hole::instantiate(match *m) {
	if(m == NULL) {
		cerr << "ERROR: hole::apply_match(): Invalid place graph contains a hole." << endl;
		exit(1);
	}

	term *t = m->get_param(index);

	if(DEBUG) cout << "BUG: hole::instantiate(): " << t << endl;
	if(DEBUG) cout << "BUG: hole::instantiate(): " << t->to_string() << endl;

	// we can safely instantiate the parameter using instantiate(), because it should be ground.
	return t->instantiate(NULL);
}

unsigned int hole::size() {
	return 1;
}

void hole::accept(termvisitor *t) {
	t->visit(this);
}

nil::nil() {
	type = TNIL;
}

nil::~nil() {
}

string nil::to_string() {
	return "nil";
}

set<match *> nil::find_matches(match *m) {
	if(DEBUG) cout << "BUG: nil::find_matches()" << endl;

	list<term *> r = m->remaining();

	if(r.size() == 0 || (r.size() == 1 && r.front()->type == TNIL)) {
		m->success();
		m->advance(NULL,list<term*>());
		return match::singleton(m);
	}


	if(r.size() == 1 && r.front()->type == THOLE) {
		m->add_param(((hole *)r.front())->index, this);
		m->success();
		m->advance(NULL,list<term*>());
		return match::singleton(m);
	}

	m->failure();

	return set<match *>();
} 

term *nil::apply_match(match *m) {
	return new nil();
}

term *nil::instantiate(match *m) {
	return new nil();
}

unsigned int nil::size() {
	return 0;
}

void nil::accept(termvisitor *t) {
	t->visit(this);
}

list<term *> term::singleton(term *t) {
	list<term *> l;
	l.push_back(t);
	return l;
}

term *term::next() {
	if(remaining.size() == 0) return NULL;

	term *t = remaining.front();
	remaining.pop_front();

	switch(t->type) {
		case TPREF:
		remaining.push_back(((prefix *)t)->get_suffix());
		break;
		case TPAR: {
			set<term*> ch = ((parallel *)t)->get_children();
			set<term*>::iterator i = ch.begin();
			while(i != ch.end())
				remaining.push_back(*(i++));
			break;
		}
		case THOLE:
			break;
		case TNIL:
			break;
		default:
			cerr << "Matching encountered invalid term type " << t->type << endl;
			exit(1);
			break;
	}

	return t;
}

void term::reset() {
	remaining.clear();
	remaining.push_back(this);
}

set<match *> term::find_all_matches(term *t, reactionrule *r) {
	set<match *> matches;

	term *p = t->next();
	while(p != NULL) {
		if(DEBUG) cout << "P: " << p->to_string() << endl;
		match *newmatch = new match(NULL, term::singleton(r->redex), NULL, r);
		matches = match::merge(matches, p->find_matches(newmatch));
		if(DEBUG) cout << "BUG: term::find_all_matches(): " << newmatch->has_succeeded << "p: " << p->to_string() << endl;
		p = t->next();
	}

	t->reset();

	return matches;
}

