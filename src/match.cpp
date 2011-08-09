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
#include <iostream>

#include <bigmc.h>

#include <assert.h>

long u_match = 1;

match::match(reactionrule *rl) {
	root = NULL;

	if(DEBUG) cout << "BUG: match::match(): new match created" << endl;

	parent = NULL;
	rule = rl;
	has_failed = false;
	has_succeeded = false;
	mc::match_mark_delete(this);
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
	if(parameters.find(id) == parameters.end()) return NULL;

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

void match::set_rule(reactionrule *r) {
	rule = r;
}

term *match::get_mapping(term *targ) {
	if(mapping.find(targ) == mapping.end()) return NULL;

	return mapping[targ];
}

map<term *,term *> match::get_mappings() {
	return mapping;
}

void match::capture_name(name src, name target) {
	names[src] = target;

	if(parent != NULL)
		parent->names[src] = target;
}

map<name,name> match::get_names() {
	if(parent != NULL) return parent->names;

	return names;
}

name match::get_name(name n) {
	if(n == 0) return 0;

	if(!bigraph::is_free(n)) return n;

	if(parent == NULL) {
		return names[n];
	}

	return parent->get_name(n);
}

set<match *> match::failure() {
	has_failed = true;
	has_succeeded = false;
	mc::match_mark_delete(this);
	return set<match *>();
}

void match::success() {
	if(has_failed) {
		cerr << "BUG: match::success(): match has already failed!" << endl;
		exit(1);
	}
	
	has_succeeded = true;
	has_failed = false;
}

set<match *> match::singleton(match *t) {
	set<match *> l;
	l.insert(t);
	return l;
}

list<term *> match::remaining() {
}

match *match::clone() {
	if(DEBUG) cout << "BUG: match::clone(): new match created" << endl;
	match *m = new match(rule);
	m->parameters = parameters;
	m->mapping = mapping;
	m->has_succeeded = has_succeeded;
	m->has_failed = has_failed;
	m->root = root;
	m->names = names;

	if(m->root)
		if(DEBUG) cout << "BUG: match::clone(): setting root: " << m->root->to_string() << endl;
	return m;
}

match *match::fresh(term *head, list<term *> rem) {
	if(DEBUG) cout << "BUG: match::fresh(): new match created" << endl;
	return new match(rule);
}

void match::advance(term *head, list<term *> rem) {
}

set<match *> match::merge(set<match *> a, set<match *> b) {
	if(DEBUG) cout << "BUG: match::merge(): " << a.size() << " with " << b.size() << endl;
	set<match *> n;
	n.insert(a.begin(),a.end());
	n.insert(b.begin(),b.end());
	return n;
}

string match::to_string() {
	string s = "Match: ";

	if(root == NULL) 
		s += "Root: NULL";
	else
		s += "Root: " + root->to_string();

	if(has_succeeded) {
		s += " Succeeded";
	}

	if(has_failed) {
		s += " Failed";
	}

	s += "\nParameters:\n";

	for(map<int,term*>::iterator i = parameters.begin(); i!=parameters.end(); i++) {
		std::stringstream p;
		p << i->first;
		s += "\t" + p.str() + ": " + ((i->second != NULL) ? i->second->to_string() : "nil") + "\n";
	}

	s += "Term Mapping:\n";

	for(map<term*,term*>::iterator i = mapping.begin(); i!=mapping.end(); i++) {
		s += "\t" + i->first->to_string() + " -> " + i->second->to_string() + "\n";

	}

	s += "Link Mapping:\n";

	map<name,name> pn;

	if(parent != NULL)
		pn = parent->names;
	else
		pn = names;

	for(map<name,name>::iterator i = pn.begin(); i!=pn.end(); i++) {
		std::stringstream p;
		p << i->first << " -> " << i->second;
		s += "\t" + bigraph::name_to_string(i->first) + " -> " + bigraph::name_to_string(i->second) + 
			"(" + p.str() + ")\n";

	}

	return s;
}

void match::incorporate(match *other) {
	for(map<int,term*>::iterator i = other->parameters.begin(); i!=other->parameters.end(); i++) {
		parameters[i->first] = i->second;
	}

	for(map<term*,term*>::iterator i = other->mapping.begin(); i!=other->mapping.end(); i++) {
		mapping[i->first] = i->second;
	}

	for(map<name,name>::iterator i = other->names.begin(); i!=other->names.end(); i++) {
		capture_name(i->first,i->second);
	}

}

const bool match::is_wide() {
	return false;
}

wide_match::wide_match(reactionrule *r)
: match(r)
{

}

wide_match::~wide_match() {
	list<match *>::iterator i = submatches.begin();

	while(i != submatches.end()) {
		mc::match_mark_delete(*i);
		i++;
	}

	submatches.clear(); // FIXME: leaking matches?
}

void wide_match::add_submatch(match *m) {
	submatches.push_back(m);
	incorporate(m);
	m->parent = this;
}

list<match *> wide_match::get_submatches() {
	return submatches;
}

match *wide_match::clone() {
	wide_match *m = new wide_match(get_rule());
	m->submatches = submatches;
	m->names = names;
	m->parameters = parameters;
	m->mapping = mapping;
	return m;
}

const bool wide_match::is_wide() {
	return true;
}

string wide_match::to_string() {
	string s = "WideMatch:\n";

	for(list<match *>::iterator i = submatches.begin(); i!=submatches.end(); i++) {
		s += (*i)->to_string() + "\n";
	}

	return s;
}
