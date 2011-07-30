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

#include <config.h>
#include <globals.h>
#include <report.h>
#include <match.h>
#include <term.h>
#include <reactionrule.h>
#include <bigraph.h>

long u_match = 1;

match::match(term *head, list<term *>red, match *prnt, reactionrule *rl) {
	root = NULL;

	if(DEBUG) cout << "BUG: match::match(): new match created" << endl;

	matchhead = head;
	parent = prnt;
	rule = rl;
	redex = red;
	has_failed = false;
	has_succeeded = false;
	u_match++;
	foobar = u_match;
	//id = u_match++;
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

void match::set_rule(reactionrule *r) {
	rule = r;
}

term *match::get_mapping(term *targ) {
	return mapping[targ];
}

void match::capture_name(name src, name target) {
	names[src] = target;
}

map<name,name> match::get_names() {
	return names;
}

set<match *> match::failure() {
	has_failed = true;
	has_succeeded = false;
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
	return redex;
}

match *match::clone(term *head, list<term *> rem) {
	if(DEBUG) cout << "BUG: match::clone(): new match created" << endl;
	match *m = new match(head, rem, NULL, rule);
	m->parameters = parameters;
	m->mapping = mapping;
	m->has_succeeded = has_succeeded;
	m->has_failed = has_failed;
	m->root = root;
	if(m->root)
		if(DEBUG) cout << "BUG: match::clone(): setting root: " << m->root->to_string() << endl;
	return m;
}

match *match::fresh(term *head, list<term *> rem) {
	if(DEBUG) cout << "BUG: match::fresh(): new match created" << endl;
	return new match(head, rem, NULL, rule);
}

void match::advance(term *head, list<term *> rem) {
	// Update the match in-place
	matchhead = head;
	redex = rem;
}

set<match *> match::merge(set<match *> a, set<match *> b) {
	if(DEBUG) cout << "BUG: match::merge(): " << a.size() << " with " << b.size() << endl;
	set<match *> n;
	n.insert(a.begin(),a.end());
	n.insert(b.begin(),b.end());
	return n;
}

string match::to_string() {
	string s = "";

	std::stringstream out;
	out << foobar;
	s += "Match #" + out.str() + ": ";
	
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

	s += " Redex: [";

	unsigned int j = 0;
	for(list<term*>::iterator i = redex.begin(); i!=redex.end(); i++) {
		s += (*i)->to_string();
		if(j != redex.size()-1) s += ", ";
		j++;
	}

	s += "]\n";

	for(map<int,term*>::iterator i = parameters.begin(); i!=parameters.end(); i++) {
		std::stringstream p;
		p << i->first;
		s += "\t" + p.str() + ": " + i->second->to_string() + "\n";
	}

	s += "Term Mapping:\n";

	for(map<term*,term*>::iterator i = mapping.begin(); i!=mapping.end(); i++) {
		s += "\t" + i->first->to_string() + " -> " + i->second->to_string() + "\n";

	}

	s += "Link Mapping:\n";

	for(map<name,name>::iterator i = names.begin(); i!=names.end(); i++) {
		s += "\t" + bigraph::name_to_string(i->first) + " -> " + bigraph::name_to_string(i->second) + "\n";

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
}

const bool match::is_wide() {
	return false;
}

wide_match::wide_match(reactionrule *r)
: match(NULL,list<term*>(),NULL,r)
{
	;
}

wide_match::~wide_match() {

}

void wide_match::add_submatch(match *m) {
	submatches.push_back(m);
}

list<match *> wide_match::get_submatches() {
	return submatches;
}

match *wide_match::clone(term *head, list<term *> rem) {
	wide_match *m = new wide_match(get_rule());
	m->submatches = submatches;
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
