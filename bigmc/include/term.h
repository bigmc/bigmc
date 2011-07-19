#ifndef _TERM_H
#define _TERM_H

#include <string>
#include <sstream>

#define TPREF	1
#define TPAR	2
#define THOLE	4
#define TNIL	8

class term {
public:
	term();
	~term();
	virtual string to_string();
	int type;
	virtual set<match *> find_matches(reactionrule *r);
};

class parallel : public term {
	set<term *> terms;
public:
	parallel(set<term *> l);
	~parallel();
	string to_string();
	set<match *> find_matches(reactionrule *r);
	set<term *> get_children();
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
	term *get_suffix();
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


