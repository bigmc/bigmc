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
#include <iomanip>
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

		if(m->root == NULL && t->active_context()) {
			m->root = t;
		} 

		if(m->root == NULL && !t->active_context()) {
			return m->failure();
		}

		m->add_match(r,t);

		return try_match(t->get_suffix(), r->get_suffix(), m);
	} else {
		return m->failure();
	}
}

set<match *> matcher::try_match(parallel *t, prefix *r, match *m) {
	set<term *> tch = t->get_children();
	
	return set<match*>();
}

set<match *> matcher::try_match(parallel *t, parallel *r, match *m) {
	set<term *> tch = t->get_children();
	set<term *> rch = r->get_children();
	map<term *, vector<set<match *> > > matches;

	if(rch.size() > tch.size())
		return m->failure();

	// FIXME: We need to distinguish matching behaviour for:
	// a.(a | b) vs. a | b -- the former will not match a.(a | b | c), latter will.

	if(m->root == NULL) {
		if(!t->active_context()) return m->failure();
	
		m->root = t;
	}
	
	m->add_match(r,t);

	int sum = rch.size() * tch.size();


	unsigned int xdim = tch.size();
	unsigned int ydim = rch.size();
	term *cand[xdim];

	int p = 0;
	for(set<term*>::iterator i = tch.begin(); i != tch.end(); i++) {
		cand[p++] = *i;
	}

	sort(cand,cand+xdim);

	set<match *> res;

	do {
		int k = 0;
		int succ = 0;
		
		match *nn = m->clone(m->root, list<term*>());

		for(set<term *>::iterator i = rch.begin(); i != rch.end(); i++) {
			if((*i)->type == THOLE) {
				if(xdim > ydim) {
					// There are "extra" things lying around, 
					// push these all into the hole.
					set<term*> nch;
				
					nch.insert(cand[xdim-1-k++]);

					for(int l = 0; l<xdim-ydim; l++) {
						nch.insert(cand[l]);
					}

					parallel *np = new parallel(nch);
					nn->add_param(((hole*)(*i))->index, np);
					succ++;
					continue;
				} else if (xdim == ydim) {
					nn->add_param(((hole*)(*i))->index, cand[xdim-1-k++]);
					succ++;
					continue;
				} else {
					nn->failure();
					break;
				}
			}

			set<match*> mm = try_match(cand[xdim-1-k++],*i,nn);

			if(nn->has_failed) {
				delete nn;
				mm.clear();
				break;
			}

			nn->has_succeeded = false;

			res = match::merge(res,mm);		

			succ++;
		}

		if(succ >= ydim) {
			res.insert(nn);
		}
	} while(next_permutation(cand, cand+xdim));

	if(res.size() == 0) return m->failure();

	return res;
}

set<match *> matcher::try_match(term *t, regions *r, match *m) {
	return set<match*>();
}

set<match *> matcher::try_match(term *t, hole *r, match *m) {
	m->success();
	m->add_param(r->index, t);
	return match::singleton(m);
}

set<match *> matcher::try_match(nil *t, nil *r, match *m) {
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
		match *nm = new match(NULL, term::singleton(r->redex), NULL, r);
		matches = match::merge(matches, try_match(p, r->redex, nm));
		p = t->next();
	}

	t->reset();

	return matches;
}


