#ifndef _MC_H
#define _MC_H

class mc {
	graph *g;
	deque<node *> workqueue;
public:
	mc(bigraph *b);
	~mc();
	bool check();
	string report();
	bool step();
};

#endif
