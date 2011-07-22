using namespace std;
#include <string>
#include <set>
#include <map>
#include <iostream>

#include <bigmc.h>

map<string,query*> mc::properties;

mc::mc(bigraph *b) {
	node *n = new node(b,NULL,NULL);
	g = new graph(n);
	workqueue.push_back(n);
	steps = 0;
}

mc::~mc() {

}

bool mc::check() {
	// TODO:
	/*
		* Sanity check models - e.g. issue warning if redex is matched in reactum.
		* Add safety property sub language
		* Add dot output
		* 
	*/

	while(step())
		;

	return false;
}

bool mc::check_properties(node *n) {
	if(n->is_visited()) return true;

	for(map<string,query*>::iterator i = properties.begin(); i!=properties.end(); i++) {
		if(!i->second->check(n)) {
			// Failed
			cout << "*** Found violation of property: " << i->first << endl;
			cout << "*** " << i->first << ": " << i->second->to_string() << endl;
			cout << g->backtrace(n);
			return false;
		}
	}

	n->set_visited(true);
	return true;
}

string mc::report() {
	stringstream out;
	out << "[q: " << workqueue.size() << " / g: " << g->size() << "] @ " << steps;
	g->dump_dot();
	return out.str();
}

// returns true while there is work to do
bool mc::step() {
	if(steps >= g_maxsteps) {
		cout << "mc::step(): Interrupted!  Reached maximum steps: " << g_maxsteps << endl;
		cout << report() << endl;
		return false;
	}

	steps++;

	if(workqueue.size() == 0) {
		// We're done!
		cout << "mc::step(): Complete!" << endl;
		cout << report() << endl;
		// TODO: sound the alarms and release the balloons at this point.
		return false; 
	}

	node *n = workqueue.front();
	workqueue.pop_front();
	bigraph *b = n->bg;


	if(!check_properties(n)) {
		cout << "mc::step(): Counter-example found." << endl;
		return false;
	}


	set<match *> matches = b->find_matches();
	if(matches.size() == 0) {
		n->terminal = true;
		return true;
	}

	for(set<match *>::iterator it = matches.begin(); it != matches.end(); ++it) {
		bigraph *b2 = b->apply_match(*it);
		node *n2 = new node(b2,n,(*it)->get_rule());

		node *n3 = g->get(n2->hash);
		if(n3 != NULL) {
			delete n2;
			n2 = n3;
		} else {
			workqueue.push_back(n2);
			g->add(n2);
		}
		
		n->add_target(n2,(*it)->get_rule());

		if(g_debug) cout << "BUG: mc::step(): new node" << endl << b2->to_string() << endl;	
		if(g_debug) cout << "BUG: mc::step(): workq size: " << workqueue.size() << endl;
	}

	if(g_report_interval > 0 && steps % g_report_interval == 0)
		cout << report() << endl;

	return true;
}

void mc::add_property(string s,query *q) {
	properties[s] = q;
}
