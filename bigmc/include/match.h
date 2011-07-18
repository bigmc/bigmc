#ifndef _MATCH_H
#define _MATCH_H

class term;
class reactionrule;

class match {
	map<int,term *> parameters;
	map<term *,term *> mapping;
	reactionrule *rule;
public:
	match(term *r, reactionrule *rl);
	~match();
	void add_param(int id, term *c);
	term *get_param(int id);
	void add_match(term *src, term *target);
	term *get_mapping(term *src);
	reactionrule *get_rule();
	term *root;
	
};

#endif

