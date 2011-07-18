using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <bigmc.h>

match::match(term *r, reactionrule *rl) {
	root = r;
	rule = rl;
}

match::~match() {
}

void match::add_param(int id, term *c) {
	if(c == NULL) {
		cout << "BUG: attempted to add_param for null term in hole " << id << endl;
		exit(1);
	}

	parameters[id] = c;
}

term *match::get_param(int id) {
	return parameters[id];
}

void match::add_match(term *src, term *target) {
	if(src == NULL || target == NULL) {
		cout << "BUG: attempted to add null match" << endl;
		exit(1);
	}

	mapping[target] = src;
}

reactionrule *match::get_rule() {
	return rule;
}

term *match::get_mapping(term *targ) {
	return mapping[targ];
}
