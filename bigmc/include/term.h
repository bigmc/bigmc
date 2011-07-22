#ifndef _TERM_H
#define _TERM_H

#include <string>
#include <sstream>
#include <deque>

#define TPREF	1
#define TPAR	2
#define THOLE	4
#define TNIL	8

class termvisitor;

class term {
protected:
	deque<term *> remaining;
public:
	term();
	~term();
	virtual string to_string();
	int type;
	virtual set<match *> find_matches(match *m);
	virtual term *apply_match(match *m);
	virtual term *instantiate(match *m);
	static list<term *> singleton(term *t);
	term *next();
	void reset();
	static set<match *> find_all_matches(term *t, reactionrule *m);
	virtual unsigned int size();
	virtual void accept(termvisitor *t);
	term *parent;
	virtual bool active_context();
};

class parallel : public term {
	set<term *> terms;
public:
	parallel(set<term *> l);
	~parallel();
	string to_string();
	set<term *> get_children();
	set<match *> find_matches(match *m);
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);
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
	set<match *> find_matches(match *m);
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);
	bool active_context();
};

class hole : public term {
public:
	hole(int idx);
	~hole();
	string to_string();
	set<match *> find_matches(match *m);
	int index;
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);

};

class nil : public term {
public:
	nil();
	~nil();
	string to_string();
	set<match *> find_matches(match *m);
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);

};

class termvisitor {
public:
	virtual void visit(term *t);
	virtual void visit(parallel *t);
	virtual void visit(prefix *t);
	virtual void visit(hole *t);
	virtual void visit(nil *t);
};

#endif


