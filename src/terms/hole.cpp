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

hole::hole(int idx) {
	index = idx;
	type = THOLE;
	id = term::u_term++;
}

hole::~hole() {
}

string hole::to_string() {
	if(index >= 19191911) return "$ctx";


	std::string s;
	std::stringstream out;
	out << index;
	return "$" + out.str();
}

set<match *> hole::find_matches(match *m) {
	if(DEBUG) cout << "BUG: hole::find_matches()" << endl;

	m->advance(NULL,list<term*>());
	return set<match *>();
} 

term *hole::apply_match(match *m) {
	// Erm, no.  There shouldn't be any holes in the place graph.
	if(DEBUG) cerr << "ERROR: hole::apply_match(): Invalid place graph contains a hole." << endl;
	exit(1);
	return NULL;
}

term *hole::instantiate(match *m) {
	if(m == NULL) {
		cerr << "ERROR: hole::apply_match(): Invalid place graph contains a hole." << endl;
		exit(1);
	}

	if(DEBUG) rinfo("hole::instantiate") << m->to_string() << endl;

	term *t = m->get_param(index);

	if(t == NULL)
		t = new nil();

	if(DEBUG) cout << "BUG: hole::instantiate(): $" << index << " with " << t << endl;
	if(DEBUG) cout << "BUG: hole::instantiate(): " << t->to_string() << endl;

	// we can safely instantiate the parameter using instantiate(), because it should be ground.
	return t->instantiate(NULL);
}

unsigned int hole::size() {
	return 1;
}

void hole::accept(termvisitor *t) {
	t->visit(this);
}

