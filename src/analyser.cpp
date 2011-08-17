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

// For a property like size() > 10, we can come up with a size trajectory metric.
// If only reaction rules that grow the size are able to be applied, we need not check
// anything with size() > 11.

// For match rules, we know the alphabet A of the match.  If the initial alphabet
// of the agent is B, for any branch that has reaction rules that can only introduce
// elements of C (not in A), then we know inductively that the match will never apply
// to that branch, and so we can stop checking it.

// The general strategy is to come up with some rule attributes P that are not satisfied
// at the root, and then prune any branches for which P(x+1) can never be true
// (because no rule can ever make P true), because we know that branch will never violate
// the property.

// The best metrics are the alphabet of controls (for match), a lower bound on
// the size of the term (for size and empty), and the depth of the term (also for match).
// If a match redex is x places deep, then an agent of depth y (where y < x) is never
// going to violate that property.

// I haven't thought of a good way to check terminal properties.

void analyser::interference() {
	set<reactionrule *> rules = bg->get_rules();

	map<reactionrule *, map<reactionrule *, set<control> > > infset;

	for(set<reactionrule*>::iterator k = rules.begin(); k!=rules.end(); k++) {
		ruleattr *a = analyse(*k);
		cout << a->to_string() << endl;
	}
}

ruleattr *analyser::analyse(reactionrule *r) {
	controlvisitor *c1 = new controlvisitor();
	controlvisitor *c2 = new controlvisitor();

	r->redex->accept(c1);
	r->reactum->accept(c2);

	ruleattr *a = new ruleattr();

	a->rule = r;

	delta_t sd;
	sd.delta = c2->size - c1->size;
	
	delta_t dd;
	dd.delta = c2->depth - c1->depth;

	// FIXME: need to check for cases like:
	// a.($0 | b.$1) -> a.($0 | $0) for validity
	if(c1->nholes == c2->nholes) {
		sd.valid = true;
		dd.valid = true;
	} else if (c2->nholes < c1->nholes) {
		sd.valid = true;
		dd.valid = true;
	} else {
		sd.valid = false;
		dd.valid = false;
	}

	a->size_delta = sd;
	a->depth_delta = dd;

	a->precond = c1->controls;
	a->postcond = c2->controls;

	delete c1;
	delete c2;
	
	return a;
}

controlvisitor::controlvisitor() {
	nholes = 0;
	size = 0;
	depth = 0;
}

controlvisitor::~controlvisitor() {

}

void controlvisitor::visit(term *t) {
	size++;
}

void controlvisitor::visit(parallel *t) {

}

void controlvisitor::visit(regions *t) {
	
}

void controlvisitor::visit(prefix *t) {
	controls.insert(t->get_control());
	size++;
	depth++;
}

void controlvisitor::visit(hole *t) {
	nholes++;
	size++;
	depth++;
}

void controlvisitor::visit(nil *t) {
	depth++;
}



