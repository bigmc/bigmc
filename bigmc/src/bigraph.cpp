using namespace std;
#include <string>
#include <set>
#include <map>
#include <iostream>

#include <bigmc.h>


control bigraph::u_control = 1;
name bigraph::u_name = 1;
map<string,name> bigraph::name_map;
map<string,control> bigraph::control_map;
map<control,bool> bigraph::activity_map;
map<control,int> bigraph::arity_map;


bigraph::bigraph(int roots = 1) {
	root = NULL;
	nregions = 1;
}

bigraph::~bigraph() {
	// should maybe recursively delete nodes
}

control bigraph::control_from_string(string n) {
	map<string,control>::iterator it;
	it = bigraph::control_map.find(n);
	if(it == bigraph::control_map.end()) {
		// No such element, insert a new one
		control fresh = bigraph::u_control++;
		bigraph::control_map[n] = fresh;
		return fresh;
	} else {
		return bigraph::control_map[n];
	}
}

name bigraph::name_from_string(string n) {
	map<string,name>::iterator it;
	it = bigraph::name_map.find(n);
	if(it == bigraph::name_map.end()) {
		// No such element, insert a new one
		name fresh = bigraph::u_name++;
		bigraph::name_map[n] = fresh;
		return fresh;
	} else {
		return bigraph::name_map[n];
	}
}

bool bigraph::activity(control c) {
	return bigraph::activity_map[c];
}

int bigraph::arity(control c) {
	return bigraph::arity_map[c];
}

control bigraph::add_control(string n, bool act, int ar) {
	control f = bigraph::control_from_string(n);
	bigraph::activity_map[f] = act;
	bigraph::arity_map[f] = ar;
	return f;
}

void bigraph::add_outer_name(name n) {
	outer.insert(n);
}

void bigraph::add_inner_name(name n) {
	inner.insert(n);
}

void bigraph::set_root(node *n) {
	root = n;
}

void bigraph::add_rule(reactionrule *r) {
	rules.insert(r);
}

string bigraph::control_to_string(control c) {
	map<string,control>::iterator it = bigraph::control_map.begin();

	while(it != control_map.end()) {
		if(it->second == c)
			return it->first;

		++it;
	}

	return "<unknown control>";
}

string bigraph::name_to_string(name c) {
	map<string,name>::iterator it = bigraph::name_map.begin();

	while(it != name_map.end()) {
		if(it->second == c)
			return it->first;

		++it;
	}

	return "<unknown control>";
}

node * bigraph::get_root(int i) {
	return root;
}

set<match *> bigraph::find_matches() {
	set<match *> res;

	for(set<reactionrule *>::iterator i = rules.begin(); i!= rules.end(); ++i) {
		cout << "bigraph::find_matches(): redex: " << (*i)->redex->to_string() << endl;
		set<match *> m = get_root(0)->find_matches(*i);
		res.insert(m.begin(),m.end());
	}

	return res;
}

bigraph *bigraph::apply_match(match *m) {
	bigraph *b = new bigraph(1);

	set<node *> matches = root->apply_match(m);

	if(matches.size() != 1) {
		cout << "Error: matching returned " << matches.size() << " nodes instead of 1!" << endl;
		exit(1);
	}

	set<node *>::iterator i = matches.begin();

	b->root = *i;

	// destroy the match -- we're done with it
	delete m;
	return b;
}

