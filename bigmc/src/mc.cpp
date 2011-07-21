using namespace std;
#include <string>
#include <set>
#include <map>
#include <iostream>

#include <bigmc.h>

mc::mc(bigraph *b) {
	node *n = new node(b,NULL);
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
	return false;
}

string mc::report() {
	return ".";
}

// returns true while there is work to do
bool mc::step() {
	if(steps >= g_maxsteps) {
		cout << "mc::step(): Interrupted!  Reached maximum steps: " << g_maxsteps << endl;
		return false;
	}

	steps++;

	if(workqueue.size() == 0) {
		// We're done!
		cout << "mc::step(): Complete!" << endl;
		// TODO: sound the alarms and release the balloons at this point.
		return false; 
	}

	node *n = workqueue.front();
	workqueue.pop_front();
	bigraph *b = n->bg;

	set<match *> matches = b->find_matches();
	if(matches.size() == 0) {
		n->terminal = true;
		return true;
	}


	for(set<match *>::iterator it = matches.begin(); it != matches.end(); ++it) {


		bigraph *b2 = b->apply_match(*it);
		node *n2 = new node(b2,n);
		g->add(n2);
		n->add_target(n2,(*it)->get_rule());
		workqueue.push_back(n2);
		if(g_debug) cout << "BUG: mc::step(): new node" << endl << b2->to_string() << endl;	
		if(g_debug) cout << "BUG: mc::step(): workq size: " << workqueue.size() << endl;


	}

	if(steps % 100 == 0)
		cout << "[q: " << workqueue.size() << " / g: " << g->size() << "] @ " << steps << endl;

	return true;
}
