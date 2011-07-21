using namespace std;
#include <string>
#include <set>
#include <iostream>
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
