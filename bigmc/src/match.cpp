using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <bigmc.h>

match::match(term *r, reactionrule *rl) {
	root = r;
	rule = rl;
	remaining.push_back(rule->redex);
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

// Get the next thing to match.
// Returns null when nothing is left to match.
term *match::next() {
	if(remaining.size() == 0) return NULL;

	term *t = remaining.front();
	remaining.pop_front();

	switch(t->type) {
		case TPREF:
		remaining.push_back(((prefix *)t)->get_suffix());
		break;
		case TPAR: {
			set<term*> ch = ((parallel *)t)->get_children();
			set<term*>::iterator i = ch.begin();
			while(i != ch.end())
				remaining.push_back(*(i++));
			break;
		}
		case THOLE:
			break;
		case TNIL:
			break;
		default:
			cerr << "Matching encountered invalid term type " << t->type << endl;
			exit(1);
			break;
	}

	return t;
}
