using namespace std;
#include <string>
#include <set>
#include <iostream>

#include <bigmc.h>

node::node(bigraph *b, node *psrc, reactionrule *r) {
	parent = psrc;
	bg = b;
	visited = false;
	terminal = false;
	rule = r;

	// FIXME: This is a kludge
        
	// sdbm hash over the to_string() of the bigraph (!!!!!!!!!!!)
	hash = 0;
        int c;
	string s = b->to_string();

	int i = 0;
        while ((c = s[i++]) != '\0')
            hash = c + (hash << 6) + (hash << 16) - hash;
}

node::~node() {

}

void node::add_target(node *n, reactionrule *r) {
	target.insert(pair<node*,reactionrule*>(n,r));
}

void node::set_visited(bool v) {
	visited = v;
}

bool node::is_visited() {
	return visited;
}

