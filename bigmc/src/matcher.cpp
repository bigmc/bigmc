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
#include <bigraph.h>
#include <matcher.h>

set<match *> matcher::try_match(prefix *t, prefix *r, match *m) {
	if(t->get_control() == r->get_control()) {
		cout << "prefix: successful match" << endl;

		if(m->root == NULL && t->active_context()) {
			m->root = t;
		} 

		if(m->root == NULL && !t->active_context()) {
			cout << "prefix: match failed: passive ctx" << endl;
			return m->failure();
		}

		m->add_match(r,t);

		return try_match(t->get_suffix(), r->get_suffix(), m);
	} else {
		cout << "prefix: match failed" << endl;
		return m->failure();
	}
}

set<match *> matcher::try_match(parallel *t, prefix *r, match *m) {
	cout << "matcher::try_match: parallel, prefix" << endl;
	set<term *> tch = t->get_children();
	
	return set<match*>();
}

set<match *> matcher::try_match(parallel *t, parallel *r, match *m) {
	set<term *> tch = t->get_children();
	set<term *> rch = r->get_children();
	map<term *, list<set<match *> > > matches;

	if(rch.size() > tch.size())
		return m->failure();

	m->add_match(r,t);

	for(set<term *>::iterator i = rch.begin(); i != rch.end();  i++) {
		for(set<term *>::iterator j = tch.begin(); j != tch.end(); j++) {
			cout << "matcher: " << (*i)->to_string() << " against " << (*j)->to_string() << endl;
			match *nm = m->clone(m->root, term::singleton(*i));
			set<match*> mt = try_match(*j,*i,nm);

			if(nm->has_succeeded || mt.size() > 0) {
				matches[*i].push_back(mt);
				cout << "nm succeeded: " << mt.size() << " m: " << nm->to_string() << endl;
			} else {
				//cout << "nm: failed: " << nm->to_string() << endl;
			}

			//cout << "matcher: " << matches.size() << endl;
		} 
	}

	if(matches.size() < rch.size()) {
		// We didn't obtain enough matches for all the parts of the redex
		cout << "matches sz: " << matches.size() << " rch sz: " << rch.size() << endl;
		return m->failure();
	}

	// So we now have a table like this for a term A | A | B | C redex A | B:
	//    A   A   B   C
	// A  m1  m2  -   -
	// 
	// B  -   -   m5  -
	// 
	// match1 = m1 + m5, match2 = m2 + m5
	

	unsigned int sum = 1;
	for(set<term *>::iterator i = rch.begin(); i != rch.end();  i++) {
		sum = sum * matches[*i].size();
	}

	cout << "sum: " << sum << endl;

	return set<match*>();
}

set<match *> matcher::try_match(term *t, regions *r, match *m) {
	cout << "matcher::try_match: term, regions" << endl;
	return set<match*>();
}

set<match *> matcher::try_match(term *t, hole *r, match *m) {
	cout << "matcher::try_match: term, hole" << endl;
	m->success();
	m->add_param(r->index, t);
	return match::singleton(m);
}

set<match *> matcher::try_match(nil *t, nil *r, match *m) {
	cout << "matcher::try_match: nil, nil" << endl;
	m->add_match(r,t);
	m->success();
	return match::singleton(m);
}

set<match *> matcher::try_match(term *t, term *r, match *m) {
	if(t->type == TPREF && r->type == TPREF)
		return try_match((prefix*)t, (prefix*)r, m);
	if(t->type == TPAR && r->type == TPREF)
		return try_match((parallel*)t, (prefix*)r, m);
	if(t->type == TPAR && r->type == TPAR)
		return try_match((parallel*)t, (parallel*)r, m);
	if(r->type == TREGION)
		return try_match(t, (regions*)r, m);
	if(r->type == THOLE)
		return try_match(t, (hole*)r, m);
	if(t->type == TNIL && r->type == TNIL)
		return try_match((nil*)t, (nil*)r, m);

	return set<match *>();
}

set <match *> matcher::try_match(term *t, reactionrule *r) {
	set<match *> matches;

	term *p = t->next();
	while(p != NULL) {
		match *nm = new match(NULL, term::singleton(r->redex), NULL, NULL);
		matches = match::merge(matches, try_match(p, r->redex, nm));
		p = t->next();
	}

	t->reset();

	return matches;
}


