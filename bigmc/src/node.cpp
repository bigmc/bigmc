using namespace std;
#include <string>
#include <set>

#include <bigmc.h>

node::node() {
	parent = NULL;
	ctrl = NULL;
	arity = 0;
	active = false;
}

node::node(control c) {
	parent = NULL;
	ctrl = c;
	arity = bigraph::arity(c);
	active = bigraph::activity(c);
}

node::~node() {
	// TODO should recursively delete children
}

void node::add(node *n) {
	children.insert(n);
	n->set_parent(this);
}

node *node::get_parent() {
	return parent;
}

set<node *> node::get_children() {
	return children;
}

int node::get_arity() {
	return arity;
}

void node::set_port(int i, name n) {
	port[i] = n;
}

name node::get_port(int i) {
	return port[i];
}

void node::set_parent(node *n) {
	parent = n;
}

string node::to_string() {
	string nm = "";

	if(ctrl == 0) {
		nm = "Region_0";
	} else {
		nm = bigraph::control_to_string(ctrl);
	}

	string s = nm;
	set<node *> ch = get_children();

	if(get_children().size() == 0) return s;

	s += " {\n";

	set<node *>::iterator it = ch.begin();
	while(it != ch.end()) {
		s += (*it)->to_string() + "\n";
		++it;
	}

	return s + "}\n";
}
