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
	if(g_debug) cout << "BUG: bigraph::control_from_string(): " << n << endl;
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

void bigraph::set_root(term *n) {
	root = n;
}

void bigraph::add_rule(reactionrule *r) {
	rules.insert(r);

	// Sanity check the rule
	set<match *> f = r->reactum->find_all_matches(r->redex, r);
	if(f.size() == 0)
		return;

	cout << "=================================================================================\n";
	cout << "Warning: the redex of reaction rule:\n";
	cout << r->to_string() << "\n";
	cout << "matches the reactum reactum of the rule itself.\n";
	cout << "This is very likely to result in an infinite state space to explore through\n";
	cout << "repeated application of this rule. You'd be better off replacing this rule, or\n";
	cout << "contributing better symbolic checking to BigMC right now.  You have been warned.\n";
	cout << "=================================================================================\n";

	// FIXME: leaking matches

	return;
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

term* bigraph::get_root(int i) {
	return root;
}

set<match *> bigraph::find_matches() {
	set<match *> res;

	for(set<reactionrule *>::iterator i = rules.begin(); i!= rules.end(); ++i) {
		if(g_debug) cout << "bigraph::find_matches(): redex: " << (*i)->redex->to_string() << endl;
		set<match *> mp = term::find_all_matches(get_root(0), *i);
		res.insert(mp.begin(),mp.end());
	}

	if(g_debug) { cout << "Matches:" << endl;
	for(set<match *>::iterator i = res.begin(); i != res.end(); i++) {
		cout << " * " << (*i) << ": " << (*i)->to_string() << endl;
	}
	}

	return res;
}

bigraph *bigraph::apply_match(match *m) {
	if(g_debug) cout << "BUG: bigraph::apply_match():\n" << m->to_string() << "\nto:\n" << to_string() << endl;
	bigraph *b = new bigraph(1);

	b->root = root->apply_match(m);
	b->inner = inner;
	b->outer = outer;
	b->rules = rules;

	// destroy the match -- we're done with it
	delete m;
	return b;
}

string bigraph::to_string() {
	string s = "Bigraph:\n\tRules:\n";

	for(set<reactionrule *>::iterator r = rules.begin(); r!=rules.end(); r++) {
		s += "\t\t" + (*r)->to_string() + "\n";
	}

	s += "\tModel:\n\t\t" + root->to_string() + "\n";

	return s;	
}

