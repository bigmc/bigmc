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
public:
	node();
	node(control c);
	~node();
	virtual void add(node *n);
	node *get_parent();
	set<node *> get_children();
	int get_arity();
	name get_port(int i);
	void set_port(int i, name n);
	void set_parent(node *n);
	virtual string to_string();
	
};


class hole : public node {
	int index;
public:
	hole(int id);
	~hole();
	void add(node *n);
	string to_string(); 
};

#endif

