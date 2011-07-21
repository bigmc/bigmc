#ifndef _NODE_H
#define _NODE_H

#include <string>
#include <sstream>

class node {
	bool visited;
public:
	node *parent;
	reactionrule *rule;
	bigraph *bg;
	set<pair<node *,reactionrule *> > target;
	node(bigraph *b, node *psrc, reactionrule *r);
	~node();
	void add_target(node *n, reactionrule *r);
	unsigned long hash;
	void set_visited(bool v);
	bool is_visited();
	bool terminal;
};

#endif

