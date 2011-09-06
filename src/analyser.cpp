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
		cout << "Rule: " << (*k)->to_string() << endl;
		for(set<reactionrule*>::iterator j = rules.begin(); j!=rules.end(); j++) {
			if(interferes((*k)->reactum, (*j)->redex)) {
				cout << " + Can cause: " << (*j)->to_string() << endl;
				(*k)->causes(*j);
			} else {
				cout << " - Cannot cause: " << (*j)->to_string() << endl;
			}
		}
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

set<term *> all_children_of(term *p) {
	switch(p->type) {
		case TPAR:
			return ((parallel*)p)->get_children();
		case TPREF: {
			set<term*> r;
			r.insert(((prefix*)p)->get_suffix());
			return r;
		}
		case TREGION: {
			set<term*> r;
			list<term *> c = ((regions*)p)->get_children();
			r.insert(c.begin(), c.end());
			return r;
		}
		default:
			return set<term*>();
	}
}

list<list<term *> > all_paths(term *t) {
	list<list<term *> > partial;

	set<term*> ch = all_children_of(t);

	if(ch.size() == 0) {
		list<term*> l;
		l.push_back(t);
		partial.push_back(l);
		return partial;
	}

	for(set<term*>::iterator c = ch.begin(); c != ch.end(); c++) {
		list<list<term*> > paths = all_paths(*c);
		
		for(list<list<term *> >::iterator i = paths.begin(); i!=paths.end(); i++) {
			if(t->type != TPAR && t->type != TREGION) {
				i->push_front(t);
			}
			partial.push_back(*i);
		}
	}

	return partial;
}

bool pref_cmp(term *t, term *u, bool subs, bool pref) {
	if(t->type == TNIL && u->type == TNIL) return true;

	//if(subs && t->type == THOLE) return true;

	//if(pref && u->type == THOLE) return true;

	if(t->type == THOLE && u->type == THOLE) return true;

	if(t->type != TPREF || u->type != TPREF) return false;

	return ((prefix*)t)->get_control() == ((prefix*)u)->get_control();
}

bool path_cmp(list<term *> p, const list<term *>& q, list<term*>::iterator j, bool subs, bool pref) {
	list<term *>::iterator i = p.begin();
	
	while(i != p.end()) {
		if(j == q.end()) return false;

		if(DEBUG)
			cout << "path_cmp: " << (*i)->to_string() << " with " << (*j)->to_string() << ": ";

		if((*i)->type == THOLE || (q.size() == 1 && (*j)->type == THOLE)) { 
			if(DEBUG) cout << "Only holes!" << endl;
			return true;
		}

		if(!pref_cmp(*i,*j,subs,pref)) {
			if(DEBUG) cout << "Path does not match" << endl;
			return false;
		} else {
			if(DEBUG) cout << "Path matches" << endl;
		}

		i++;
		j++;
	}

	//cout << "Escaped loop: " << (j == q.end()) << endl;
	return j == q.end();
}

bool path_exists(list<term *> t, list<list<term *> > l) {
	for(list<list<term *> >::iterator i = l.begin(); i!=l.end(); i++) {
		for(list<term*>::iterator j = i->begin(); j != i->end(); j++) {
			if(path_cmp(t, *i, j, true, true)) {
				return true;
			}
		}
	}

	return false;
}

bool analyser::orthogonalas(term *r1, term *r2) {
	/* R \perp' R' \defeq & \lnot\exists p.nil \in P(R). p.nil \in \suff (R') \land \\
	 & \lnot\exists p \in \pref (R). p\$n \in \suff (R') \land \\
	 &\lnot\exists p\$n \in P(R). p \in \subs (R') \land \\
	 &\lnot\exists p\$n \in P(R). p\$n \in \suff (R') */
	
	if(r1->type == TREGION || r2->type == TREGION) return false;
	if(r1->type == THOLE || r2->type == THOLE) return false;
	
	list<list<term*> > p1 = all_paths(r1);
	list<list<term*> > p2 = all_paths(r2);



	for(list<list<term *> >::iterator p = p1.begin(); p != p1.end(); p++) {
		if(path_exists(*p, p2)) {
			return false;
		}
	}

	return true;
}

bool analyser::orthogonal(term *r1, term *r2) {
	return orthogonalas(r1,r2) && orthogonalas(r2,r1);
}

// Determines whether some redex could possibly be found in an agent consisting of a reactum and
// given a decomposition C o R' o d
bool analyser::interferes(term *reactum, term *redex) {
	return !orthogonal(reactum, redex);
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



