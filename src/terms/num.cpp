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

num::num(int val) {
	value = val;
	type = TNUM;
	id = term::u_term++;
}

num::~num() {
}

string num::to_string() {
	std::string s;
	std::stringstream out;
	out << value;
	return "int:" + out.str();
}

set<match *> num::find_matches(match *m) {
	if(DEBUG) cout << "BUG: num::find_matches()" << endl;

	m->advance(NULL,list<term*>());
	return set<match *>();
} 

term *num::apply_match(match *m) {
	if(id == m->root->id) {
		// This is the match site.  Return the reactum.
		if(DEBUG) cout << "BUG: num::apply_match(): Found match site!" << endl;
		term *r = m->get_rule()->reactum->instantiate(m);
		if(DEBUG) cout << "Returning reactum: " << r->to_string() << endl;
		return r;
	} else {
		if(DEBUG) cout << "BUG: num::apply_match(): not match site: " << this << " != " << m->root << endl;
		term *n = new num(value);
		n->id = id;
		return n;
	}
}

term *num::instantiate(match *m) {
	term* n = new num(value);
	n->id = id;
	return n;
}

unsigned int num::size() {
	return 1;
}

void num::accept(termvisitor *t) {
	t->visit(this);
}


