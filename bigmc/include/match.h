#ifndef _MATCH_H
#define _MATCH_H

class node;

class match {
	map<int,node *> parameters;
	map<node *,node *> mapping;
	node *root;
public:
	match(node *r);
	~match();
	void add_param(int id, node *c);
	node *get_param(int id);
	void add_match(node *src, node *target);
	
};

#endif

