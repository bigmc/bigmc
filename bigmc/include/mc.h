#ifndef _MC_H
#define _MC_H

class mc {
	bigraph *bg;
public:
	mc(bigraph *b);
	~mc();
	bool check();
	string report();
};

#endif
