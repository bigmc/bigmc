#ifndef _TERM_H
#define _TERM_H

#include <string>
#include <sstream>

class term {

public:
	term();
	~term();
	virtual string to_string();
};

class parallel : public term {
	set<term *> terms;
public:
	parallel(set<term *> l);
	~parallel();
	string to_string();
};

class prefix : public term {
	control ctrl;
	bool active;
	int arity;
	static int u_term;
	vector<name> port;
	term *suffix;
public:
	prefix(control ctrl, vector<name> ports, term *suff);
	~prefix();
	string to_string();
};

class hole : public term {
	int index;
public:
	hole(int idx);
	~hole();
	string to_string();
};

class nil : public term {
public:
	nil();
	~nil();
	string to_string();
};

#endif


