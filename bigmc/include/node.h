#ifndef _NODE_H
#define _NODE_H

class node {
	control ctrl;
	node *parent;
	set<node *> children;
	vector<name> port;
	bool active;
	int arity;
public:
	node();
	node(control c);
	~node();
	void add(node *n);
	node *get_parent();
	set<node *> get_children();
	int get_arity();
	name get_port(int i);
	void set_port(int i, name n);
	void set_parent(node *n);
	
};

#endif

