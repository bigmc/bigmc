#ifndef _MATCH_H
#define _MATCH_H

#include <deque>
#include <list>

class term;
class reactionrule;

class match {
	map<int,term *> parameters;
	map<term *,term *> mapping;
	reactionrule *rule;
	list<term *> redex;
	term *matchhead;
	match *parent;
	long foobar;
public:
	match(term *head, list<term *>rem, match *parent, reactionrule *rl);
	~match();
	void add_param(int id, term *c);
	term *get_param(int id);
	void add_match(term *src, term *target);
	term *get_mapping(term *src);
	reactionrule *get_rule();
	term *root;
	//term *next();
	set<match *> failure();
	void success();
	static set<match *> singleton(match *t);	
	list<term *> remaining();
	match *clone(term *head, list<term *> rem);
	match *fresh(term *head, list<term *> rem);
	void advance(term *head, list<term *> rem);
	static set<match *> merge(set<match *> a, set<match *> b);
	string to_string();
	bool has_succeeded;
	bool has_failed;
	void incorporate(match *other);

};

#endif

