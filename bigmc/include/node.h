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
	virtual bool is_hole();
	set<match *> find_matches(node *redex);
	virtual bool matches(node *other);
	bool is_active_context();
	static vector<node *> flatten(node *n);
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

