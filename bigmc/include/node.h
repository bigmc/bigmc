#ifndef _NODE_H
#define _NODE_H

#include <string>
#include <sstream>

class node {
	control ctrl;
	node *parent;
	set<node *> children;
	vector<name> port;
	bool active;
	int arity;
	static int u_node;
public:
	int id;
	node();
	node(control c);
	~node();
	virtual void add(node *n);
	void add(set<node *> s);
	node *get_parent();
	set<node *> get_children();
	int get_arity();
	name get_port(int i);
	void set_port(int i, name n);
	void set_parent(node *n);
	virtual string to_string();
	virtual bool is_hole();
	set<match *> find_matches(reactionrule *rule);
	virtual bool matches(node *other);
	bool is_active_context();
	static vector<node *> flatten(node *n);
	node *deep_copy();
	node *copy();
	node *instantiate(match *m);
	set<node *> apply_match(match *m);
};


class hole : public node {
public:
	int index;
	hole(int id);
	~hole();
	void add(node *n);
	string to_string(); 
	bool is_hole();
	bool matches(node *other);
};

#endif

