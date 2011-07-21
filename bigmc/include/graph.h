#ifndef _GRAPH_H
#define _GRAPH_H

#include <string>
#include <sstream>
#include <deque>

class graph {
	set<node *> nodes;
	node *root;
	map<unsigned long,node *> lut;
public:
	graph(node *r);
	~graph();
	void add(node *n);
	node *get(unsigned long id);
	void report();
	unsigned long size();
};

#endif
