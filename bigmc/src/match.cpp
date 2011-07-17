using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <bigmc.h>

match::match(node *r, reactionrule *rl) {
	root = r;
	rule = rl;
}

match::~match() {
}

void match::add_param(int id, node *c) {
	if(c == NULL) {
		cout << "BUG: attempted to add_param for null node in hole " << id << endl;
		exit(1);
	}

	parameters[id] = c;
}

node *match::get_param(int id) {
	return parameters[id];
}

void match::add_match(node *src, node *target) {
	if(src == NULL || target == NULL) {
		cout << "BUG: attempted to add null match" << endl;
		exit(1);
	}

	mapping[target] = src;
}

reactionrule *match::get_rule() {
	return rule;
}

node *match::get_mapping(node *targ) {
	return mapping[targ];
}
