#ifndef _MATCH_H
#define _MATCH_H

class node;
class reactionrule;

class match {
	map<int,node *> parameters;
	map<node *,node *> mapping;
	reactionrule *rule;
public:
	match(node *r, reactionrule *rl);
	~match();
	void add_param(int id, node *c);
	node *get_param(int id);
	void add_match(node *src, node *target);
	node *get_mapping(node *src);
	reactionrule *get_rule();
	node *root;
	
};

#endif

