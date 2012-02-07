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

parallel::parallel(set<term *>l) {
	init(term::u_term++, l);
}

parallel::parallel(unsigned long sid, set<term *>l) {
	init(sid,l);
}

void parallel::init(unsigned long sid, set<term *>l) {
	terms = l;
	type = TPAR;

	id = sid;

	for(set<term *>::iterator i = l.begin(); i!=l.end(); i++) {
		(*i)->parent = this;
	}
}

parallel::~parallel() {
	set<term*>::iterator i = terms.begin();
	while(i != terms.end()) {
		delete *i;
		i++;
	}

	terms.clear();
}

string parallel::to_string() {
	string s = "(";

	unsigned int k = 0;
	for(set<term *>::iterator i = terms.begin(); i!=terms.end(); ++i) {
		k++;

		s += (*i)->to_string();

		if(k < terms.size())
			s += " | ";
	}

	return s + ")";
}

set<match *> parallel::find_matches(match *m) {

/*
	if(DEBUG) cout << "BUG: parallel::find_matches()" << endl;

	if(m->has_succeeded || m->has_failed) return set<match *>();

	list<term *> r = m->remaining();


	if(r.size() == 0) {
		if(DEBUG) cout << "BUG: parallel::find_matches(): SUCCESS (nothing to match)" << endl;
		m->success();
		return match::singleton(m);
	}

	if(r.size() == 1 && r.front()->type == TPAR) {
		if(DEBUG) cout << "BUG: parallel::find_matches(): generating permutation" << endl;
		set<term *> c = ((parallel *)r.front())->get_children();
		list<term *> cl;
		set<match *> matches;

		if(m->root == NULL && !active_context())
			return m->failure();

		if(m->root == NULL) {
			if(DEBUG) cout << "BUG: Null root in match, setting to TPAR: " << to_string() << endl;
			m->root = this;
		}

		for(set<term*>::iterator i = c.begin(); i!=c.end(); i++)
			cl.push_back(*i);

		do {
			if(DEBUG) {
				cout << "BUG: Permutation: result set: " << matches.size() << endl;
				for(list<term *>::iterator i = cl.begin(); i!=cl.end(); i++) {
					cout << " * " << (*i)->to_string() << endl;
				}
			}

			match *mp = m->clone();
			set<match *> res = find_matches(mp);

			matches = match::merge(matches,res);
		} while(next_permutation(cl.begin(),cl.end()));

		return matches;
	}

	if(r.size() > 1) { */
		/* So we have a situation like this:
			The current node is a parallel term like this:
			( A | B | C )
			We have a permutation like this:
			[ A, B, C ]
			So we iterate over the current children (A | B | C) in-order
			and try to find matches for A and then for C.
			Once a match is "consumed", we remove it from the redex permutation
			and proceed to the next next.  There is no back-tracking or anything
			necessary.

			We can't just match directly, to avoid spoiling the
			match object.  Instead we clone it, and on a successful match,
			we incorporate the match.
		*/ /*

		if(DEBUG) { cout << "BUG: Ready to match permutation against: " << to_string() << " Permutation: ";
		for(list<term *>::iterator i = r.begin(); i!=r.end(); i++) {
			cout << (*i)->to_string() << ", ";
		}
		cout << endl;
		}

		set<match *> results;

		for(set<term *>::iterator i = terms.begin(); i!=terms.end(); i++) {
			if(DEBUG) cout << "BUG: parallel::find_matches(): iterating child: " << (*i)->to_string() << endl;
			
			if(r.size() == 0) {
				if(DEBUG) cout << "BUG: parallel::find_matches(): SUCCESS" << endl;
				// We have succeeded!  We're done!
				m->success();
				results.insert(m);
				return results;
			}

			if(r.size() == 1 && r.front()->type == THOLE) {
				// This is success.  A hole as the last
				// thing to match can match everything that is left.
				m->success();
				int idx = ((hole *)r.front())->index;
				parallel *holecontents = new parallel(set<term *>());

				while(i != terms.end()) {
					holecontents->terms.insert(*i);
					m->add_match(*i,r.front());
					i++;
				}

				if(holecontents->terms.size() == 0) {
					delete holecontents;
					m->add_param(idx,new nil());
				}

				else if(holecontents->terms.size() == 1) {
					m->add_param(idx,*(holecontents->terms.begin()));
					delete holecontents;
				}

				else {
					m->add_param(idx,holecontents);
				}

				results.insert(m);
				r.pop_front();
				return results;
			}

			term *nm = r.front();

			if(DEBUG) cout << "BUG: parallel::find_matches(): matching " << (*i)->to_string() << " against " << nm->to_string() << endl;
			// Clone the match and attempt to match
			match *cl = m->clone();

			if(DEBUG) cout << "cl: " << cl->to_string() << endl;

			set<match *> res = (*i)->find_matches(cl);

			if(cl->has_failed || !cl->has_succeeded) {
				if(DEBUG) cout << "BUG: parallel::find_matches(): match failed" << endl;
				delete cl;
				// Discard the matches we found
				// FIXME leaking matches in the set res here.
				//return m->failure();
				continue;
			} else if (cl->has_succeeded) { // Success!
				if(DEBUG) cout << "BUG: parallel::find_matches(): match succeeded" << endl;
				
				// FIXME: leaking matches in the set res here.

				m->advance(NULL, term::singleton(nm));
				m->incorporate(cl);
				match::merge(results,res);
				r.pop_front();
				delete cl;
			} 

			if(DEBUG) cout << "BUG: parallel::find_matches(): match obj: " << m->to_string() << endl;
		}

		if(r.size() == 0) {
			m->success();
			results.insert(m);
			if(DEBUG) cout << "BUG: parallel::find_matches(): match succeeded: " << m->to_string() << " Result Size: " << results.size() << endl;
			return results;
		} else {
			if(DEBUG) cout << "BUG: parallel::find_matches(): match failed: " << m->to_string() << " r.size(): " << r.size() << endl;
			if(DEBUG) cout << "BUG: r[0]: " << r.front()->to_string() << endl;
			return m->failure();
		}
	}
	
*/
	assert(0);
	return m->failure();
} 

set<term *> parallel::get_children() {
	return terms;
}

term *parallel::apply_match(match *m) {

	if(id == m->root->id) {
		if(parent == NULL && m->get_param(999999) != NULL) {
			term *r = m->get_rule()->reactum->instantiate(m);
			if(r->type != TPAR) return r;

			parallel *rp = (parallel *) r;

			term *ht = m->get_param(999999)->instantiate(NULL);

			if(ht->type == TNIL) return rp;

			if(ht->type == TPAR)
				rp->terms.insert(((parallel *)ht)->terms.begin(), ((parallel*)ht)->terms.end());
			else 
				rp->terms.insert(ht);

			return rp;
		}
	
		// This is the match site.  Return the reactum.
		if(DEBUG) cout << "BUG: parallel::apply_match(): Found match site!" << endl;
		term *r = m->get_rule()->reactum->instantiate(m);
		if(DEBUG) cout << "Returning reactum: " << r->to_string() << endl;
		return r;
	} 

	set<term *> n;

	for(set<term*>::iterator i = terms.begin(); i!=terms.end(); i++) {
		n.insert((*i)->apply_match(m));
	}

	parallel *pp = new parallel(id, n);
	pp->flatten();

	return pp;
}

term *parallel::instantiate(match *m) {
	set<term *> n;

	for(set<term*>::iterator i = terms.begin(); i!=terms.end(); i++) {
		if((*i)->type != TPAR)
			n.insert((*i)->instantiate(m));
		else {
			parallel *p = (parallel *)((*i)->instantiate(m));
			n.insert(p->terms.begin(), p->terms.end());
		}
	}

	parallel *pp = new parallel(term::u_term++,n);
	pp->flatten();
	return pp;
}

set<term*> parallel::flatten() {
	set<term *> t;

	for(set<term *>::iterator i = terms.begin(); i != terms.end(); i++) {
		if((*i)->type == TNIL) continue;

		set<term *> l = (*i)->flatten();
		t.insert(l.begin(), l.end());
	}
	
	terms = t;
	return t;
}


bool parallel::active_context() {
	if(parent == NULL) return true;

	return parent->active_context();
}

unsigned int parallel::size() {
	unsigned int sz = 0;
	for(set<term *>::iterator i = terms.begin(); i!=terms.end(); i++) {
		sz += (*i)->size();
	}

	return sz;
}

void parallel::accept(termvisitor *t) {
	for(set<term*>::iterator i = terms.begin(); i != terms.end(); i++) {
		(*i)->accept(t);
	}

	t->visit(this);
}


