#ifndef _MC_H
#define _MC_H

class mc {
	graph *g;
	deque<node *> workqueue;
	unsigned long steps;
public:
	mc(bigraph *b);
	~mc();
	bool check();
	string report();
	bool step();
};

#endif
