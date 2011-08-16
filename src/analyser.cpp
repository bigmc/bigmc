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
#include <algorithm>

#include <bigmc.h>

#include <assert.h>

analyser::analyser(bigraph *b) {
	bg = b;
}

analyser::~analyser() {

}

set<control> analyser::interference_set(set<control> s1, set<control> s2) {
	set<control> intersect;
	std::set_intersection( s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter( intersect, intersect.begin() ) );
	return intersect;	
}

// X: a -> b
// Y: b -> c
// Z: d -> e
// X interferes with Y
// Y does not interfere with X or Y
// Z does not interfere with X or Y

// 

void analyser::interference() {
	set<reactionrule *> rules = bg->get_rules();

	map<reactionrule *, map<reactionrule *, set<control> > > infset;

	for(set<reactionrule*>::iterator k = rules.begin(); k!=rules.end(); k++) {
		cout << "Rule: " << (*k)->to_string() << ": " << endl;
		for(set<reactionrule*>::iterator i = rules.begin(); i!=rules.end(); i++) {
			controlvisitor *c1 = new controlvisitor();
			controlvisitor *c2 = new controlvisitor();

			(*k)->redex->accept(c1);
			(*i)->reactum->accept(c2);

			infset[*k][*i] = interference_set(c1->controls, c2->controls);

			if(infset[*k][*i].size() == 0) {
				cout << "  * Is not interfered with by " << (*i)->to_string() << endl;
			} else {
				cout << "  * Is interfered with by " << (*i)->to_string() << endl;
			}

			delete c1;
			delete c2;
		}
	}
}


controlvisitor::controlvisitor() {
	has_holes = false;
}

controlvisitor::~controlvisitor() {

}

void controlvisitor::visit(term *t) {

}

void controlvisitor::visit(parallel *t) {

}

void controlvisitor::visit(regions *t) {

}

void controlvisitor::visit(prefix *t) {
	controls.insert(t->get_control());
}

void controlvisitor::visit(hole *t) {
	has_holes = true;
}

void controlvisitor::visit(nil *t) {

}



