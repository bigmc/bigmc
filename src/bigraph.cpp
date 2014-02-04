/*******************************************************************************
*
* Copyright (C) 2011 Gian Perrone (http://itu.dk/~gdpe)
* 
* BigMC - A bigraphical model checker (http://bigraph.org/bigmc).
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
* USA.
*********************************************************************************/
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
set<name> bigraph::names;

bigraph::bigraph(int roots) {
	root = NULL;
	nregions = 1;
}

bigraph::~bigraph() {
	delete root;
}

control bigraph::control_from_string(string n) {
	if(DEBUG) cout << "BUG: bigraph::control_from_string(): " << n << endl;
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
	if(n == "") return 0;

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
	names.insert(n);
}

void bigraph::add_inner_name(name n) {
	inner.insert(n);
	names.insert(n);
}

void bigraph::set_root(term *n) {
	root = n;
}

void bigraph::add_rule(reactionrule *r) {
	rules.insert(r);

	//r->contextify();

	return;

	if(!term::matches(r->reactum, r->redex)) return;

	rwarning("bigraph::add_rule") << "=================================================================================\n";
	rwarning("bigraph::add_rule") << "Warning: the redex of reaction rule:\n";
	rwarning("bigraph::add_rule") << r->to_string() << "\n";
	rwarning("bigraph::add_rule") << "matches the reactum of the rule itself.\n";
	rwarning("bigraph::add_rule") << "This is very likely to result in an infinite state space to explore through\n";
	rwarning("bigraph::add_rule") << "repeated application of this rule. You'd be better off replacing this rule, or\n";
	rwarning("bigraph::add_rule") << "contributing better symbolic checking to BigMC right now.  You have been warned.\n";
	rwarning("bigraph::add_rule") << "=================================================================================\n";

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
	if(c == 0) return "-";

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
		if(DEBUG) cout << "bigraph::find_matches(): redex: " << (*i)->redex->to_string() << endl;
		
		set<match *> mp = matcher::try_match(get_root(0), *i);

		//set<match *> mp = term::find_all_matches(get_root(0), *i);
		res.insert(mp.begin(),mp.end());
	}

	if(DEBUG) { cout << "Matches:" << endl;
	for(set<match *>::iterator i = res.begin(); i != res.end(); i++) {
		cout << " * " << (*i) << ": " << (*i)->to_string() << endl;
	}
	}

	return res;
}

bigraph *bigraph::apply_match(match *m) {
	if(m == NULL) return this;

	if(DEBUG) cout << "BUG: bigraph::apply_match():\n" << m->to_string() << "\nto:\n" << to_string() << endl;

	if(!m->is_wide()) {

		bigraph *b = new bigraph(1);

		b->root = root->apply_match(m);
		b->inner = inner;
		b->outer = outer;
		b->rules = rules;

		if(DEBUG) cout << "BUG: result:\n" << b->to_string() << endl;

		// destroy the match -- we're done with it
		mc::match_mark_delete(m);
		return b;
	} else {
		if(m->get_rule()->reactum->type != TREGION) {
			rerror("bigraph::apply_match") <<
				"Wide redexes must have wide reactums" << endl;
			exit(1);
		}

		regions *reactum = (regions *)m->get_rule()->reactum;
		wide_match *wm = (wide_match *)m;

		if(reactum->get_children().size() != wm->get_submatches().size()) {
			rerror("bigraph::apply_match") << "Wide rules much match in the number"
				"of regions in the redex and reactum" << endl;
			exit(1);
		}

		bigraph *b = new bigraph(1);
		b->inner = inner;
		b->outer = outer;
		b->rules = rules;
		b->root = root;

		list<match*> mq = wm->get_submatches();
		list<term *> rc = reactum->get_children();

		for(list<match*>::iterator i = mq.begin(); i!=mq.end(); i++) {
			reactionrule *nr = new reactionrule(NULL, rc.front());
			(*i)->set_rule(nr);
			rc.pop_front();

			if(DEBUG) {
				cout << "bigraph::apply_match: (wide): " << endl
					 << (*i)->to_string() << " to " << endl
					 << b->to_string() << endl;
				cout << "Bigraph B: " << b->root->to_string() << endl;
			}


			b->root = b->root->apply_match(*i);
			if(DEBUG) 
				cout << "Match M: " << (*i)->to_string() << "\nBigraph B'::\n" << b->root->to_string() << endl;

			delete nr;
		}

		mc::match_mark_delete(m);
		return b;
	}
}

string bigraph::to_string() {
	string s = "Bigraph:\n\tRules:\n";

	for(set<reactionrule *>::iterator r = rules.begin(); r!=rules.end(); r++) {
		if(*r == NULL) {
			rerror("bigraph::to_string") << "invalid NULL rule" << endl;
			exit(1);
		}

		s += "\t\t" + (*r)->to_string() + "\n";
	}

	s += "\tModel:\n\t\t" + root->to_string() + "\n";

	return s;	
}

bool bigraph::is_free(name n) {
	if(n == 0) return false;
	return (names.find(n) == names.end());
}

set<reactionrule *> bigraph::get_rules() {
	return rules;
}
