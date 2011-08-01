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
#include <map>
#include <assert.h>

#include <config.h>
#include <globals.h>
#include <report.h>
#include <match.h>
#include <term.h>
#include <reactionrule.h>

nil::nil() {
	type = TNIL;
	id = term::u_term++;
}

nil::~nil() {
}

string nil::to_string() {
	return "nil";
}

set<match *> nil::find_matches(match *m) {
	if(DEBUG) cout << "BUG: nil::find_matches()" << endl;

	list<term *> r = m->remaining();

	if(r.size() == 0 || (r.size() == 1 && r.front()->type == TNIL)) {
		m->success();
		m->advance(NULL,list<term*>());
		return match::singleton(m);
	}


	if(r.size() == 1 && r.front()->type == THOLE) {
		m->add_param(((hole *)r.front())->index, this);
		m->success();
		m->advance(NULL,list<term*>());
		return match::singleton(m);
	}

	m->failure();

	return set<match *>();
} 

term *nil::apply_match(match *m) {
	return new nil();
}

term *nil::instantiate(match *m) {
	return new nil();
}

unsigned int nil::size() {
	return 0;
}

void nil::accept(termvisitor *t) {
	t->visit(this);
}

