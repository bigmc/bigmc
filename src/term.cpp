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
#include <sstream>
#include <algorithm>
#include <assert.h>

#include <config.h>
#include <globals.h>
#include <report.h>
#include <match.h>
#include <term.h>
#include <reactionrule.h>
#include <bigraph.h>

unsigned long term::u_term = 1;

term::term() {
	type = 0;
	remaining.push_back(this);
	parent = NULL;
	id = u_term++;
}

term::~term() {
}

string term::to_string() {
	return "<untyped term>";
}

set<match *> term::find_matches(match *m) {
	assert(0);
	switch(type) {
		case TPREF: {
			cout << "term::find_matches(): prefix\n";
			break;
		} 
		case TPAR:
			cout << "term::find_matches(): par\n";
		break;
		case THOLE:
			cout << "term::find_matches(): hole\n";

		break;
		case TNIL:
			cout << "term::find_matches(): nil\n";
		break;
		default:
			cerr << "term::find_matches(): Unknown node type " << type << endl;
			exit(1);
		break;
	}

	return set<match*>();
}

term *term::apply_match(match *m) {
	assert(0);
	return this;
}

term *term::instantiate(match *m) {
	assert(0);
	return NULL;
}

unsigned int term::size() {
	return 0;
}

void term::accept(termvisitor *t) {
	t->visit(this);
}

bool term::active_context() {
	if(parent == NULL) return true;

	return parent->active_context();
}

set<term *> term::flatten() {
	if(type == TPAR)
		return ((parallel *)this)->flatten();

	set<term *> s;
	s.insert(this);
	return s; 
}



list<term *> term::singleton(term *t) {
	list<term *> l;
	l.push_back(t);
	return l;
}

term *term::next() {
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
		case TREGION: {
			list<term*> ch = ((regions *)t)->get_children();
			list<term*>::iterator i = ch.begin();
			while(i != ch.end())
				remaining.push_back(*(i++));
			break;
		}
		case THOLE:
			break;
		case TNIL:
			break;
		case TNUM:
			break;
		default:
			cerr << "Matching encountered invalid term type " << t->type << endl;
			exit(1);
			break;
	}

	return t;
}

void term::reset() {
	remaining.clear();
	remaining.push_back(this);
}

set<match *> term::find_all_matches(term *t, reactionrule *r) {
	assert(0);
	set<match *> matches;
/*
	term *p = t->next();
	while(p != NULL) {
		if(DEBUG) cout << "P: " << p->to_string() << endl;
		match *newmatch = new match(r);
		matches = match::merge(matches, p->find_matches(newmatch));
		if(DEBUG) cout << "BUG: term::find_all_matches(): " << newmatch->has_succeeded << "p: " << p->to_string() << endl;
		// FIXME: leaking newmatch here?
		p = t->next();
	}

	t->reset();
*/
	return matches;
}

bool term::matches(term *subj, term *redex) {
	assert(0);
/*	term *p = subj->next();

	while(p != NULL) {
		match *newmatch = new match(NULL, term::singleton(redex), NULL, NULL);

		set<match *> m = p->find_matches(newmatch);

		if(m.size() > 0) {
			subj->reset();
			delete newmatch;
			m.clear();
			return true;
		}
	
		m.clear();
		delete newmatch;
		p = subj->next();
	}

	subj->reset();*/

	return false;
}

bool term::overlap(term *other) {
	if(other == NULL) {
		return false;
	}

	if(other == this) return true;

	return overlap(other->parent);
}

void termvisitor::visit(term *t) {

}

void termvisitor::visit(parallel *t) {

}

void termvisitor::visit(regions *t) {

}

void termvisitor::visit(prefix *t) {

}

void termvisitor::visit(hole *t) {

}

void termvisitor::visit(nil *t) {

}

void termvisitor::visit(num *t) {

}

// This is debugging code
// It checks to see if we are accidentally sharing sub-trees within the same term.

void termconsistencyvisitor::visit(term *t) {
	if(visited.find(t) != visited.end()) {
		rerror("termconsistencyvisitor::visit") << t->to_string() << ": found duplicate sub-term." << endl;
		exit(1);
	}

	visited.insert(t);
}

void termconsistencyvisitor::visit(parallel *t) {
	if(visited.find(t) != visited.end()) {
		rerror("termconsistencyvisitor::visit") << t->to_string() << ": found duplicate sub-term." << endl;
		exit(1);
	}

	visited.insert(t);
}

void termconsistencyvisitor::visit(regions *t) {
	if(visited.find(t) != visited.end()) {
		rerror("termconsistencyvisitor::visit") << t->to_string() << ": found duplicate sub-term." << endl;
		exit(1);
	}

	visited.insert(t);
}

void termconsistencyvisitor::visit(prefix *t) {
	if(visited.find(t) != visited.end()) {
		rerror("termconsistencyvisitor::visit") << t->to_string() << ": found duplicate sub-term." << endl;
		exit(1);
	}

	visited.insert(t);
}

void termconsistencyvisitor::visit(hole *t) {
	if(visited.find(t) != visited.end()) {
		rerror("termconsistencyvisitor::visit") << t->to_string() << ": found duplicate sub-term." << endl;
		exit(1);
	}

	visited.insert(t);
}

void termconsistencyvisitor::visit(nil *t) {
	if(visited.find(t) != visited.end()) {
		rerror("termconsistencyvisitor::visit") << t->to_string() << ": found duplicate sub-term." << endl;
		exit(1);
	}

	visited.insert(t);
}

void termconsistencyvisitor::visit(num *t) {
	if(visited.find(t) != visited.end()) {
		rerror("termconsistencyvisitor::visit") << t->to_string() << ": found duplicate sub-term." << endl;
		exit(1);
	}

	visited.insert(t);
}





