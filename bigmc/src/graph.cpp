using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <bigmc.h>

graph::graph(node *initial) {
	root = initial;

	if(initial != NULL)
		nodes.insert(initial);
}

graph::~graph() {

}

void graph::add(node *n) {
	nodes.insert(n);
	lut[n->hash] = n;
}

node *graph::get(unsigned long id) {
	return lut[id];
}

void graph::report() {
	cout << "Graph Report:" << endl;
	cout << "\tNumber of nodes: " << nodes.size() << endl;
}

unsigned long graph::size() {
	return nodes.size();
}

string graph::backtrace(node *n) {
	node *h = n;

	string bt = "";
	stringstream out;

	int i = 0;

	while(h != NULL) {
		string here = "";
		if(i == 0) here = "   <- *** VIOLATION ***";
		out << "#" << i << "  " << h->bg->get_root(0)->to_string() << here << endl;
			
		if(h->rule == NULL) {
			out << " >> " << "(root)" << endl;
		} else {
			out << " >> " << h->rule->to_string() << endl << endl;
		}


		i++;
		h = h->parent;
	}

	return out.str();
}
