#ifndef _MC_H
#define _MC_H

class mc {
	graph *g;
	deque<node *> workqueue;
	unsigned long steps;
	static map<string,query *> properties;
	map<unsigned long,bool> checked;
public:
	mc(bigraph *b);
	~mc();
	bool check();
	string report(int step);
	bool step();
	static void add_property(string s, query *q);
	bool check_properties(node *n);
	static void *thread_wrapper( void *i );
};

#endif
