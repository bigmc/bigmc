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
	if(DEBUG) {
		rinfo("matcher::try_match") << "matching prefix: " << t->to_string() <<
			" against redex: " << r->to_string() << endl;
	}

	assert(m != NULL);

	if(t->get_control() == r->get_control()) {

		if(DEBUG)
		rinfo("matcher::try_match") << "Prefix match: " << t->to_string() << " with " << r->to_string() << " Active: " << t->active_context() << endl;

		if(m->root == NULL && !(t->parent == NULL || (t->parent != NULL && t->parent->active_context()))) {
			return m->failure();
		} else if(m->root == NULL) {
			m->root = t;
		}

		m->add_match(r,t);

		// Check names
		vector<name> tnm = t->get_ports();
		vector<name> rnm = r->get_ports();

		if(tnm.size() != rnm.size()) {
			if(DEBUG) cout << "matcher: tnm: " << tnm.size() << " rnm: " << rnm.size() << endl;
			return m->failure();
		}

		if(DEBUG) cout << "MATCHER LINK START" << endl;

		for(int i = 0; i<tnm.size(); i++) {
			if(DEBUG)
				cout << "MATCH: " << i << ": of term: " << t->to_string() << ". " << m->to_string() << endl;
			if(!bigraph::is_free(rnm[i])) {
				if(DEBUG) 
					cout << "matcher: !free: " << bigraph::name_to_string(rnm[i]) << endl;

				if(tnm[i] != rnm[i]) {
					if(DEBUG)
					cout << "MATCH FAILED: expected: " << bigraph::name_to_string(rnm[i]) << " got " << bigraph::name_to_string(tnm[i]) << endl;
					return m->failure();
				}

				m->capture_name(rnm[i],tnm[i]);
				if(DEBUG) 
					cout << "matcher: !free: captured " << rnm[i] << endl;
			} else {
				// This is a symbolic link name, not a literal one
				// We need to look it up in the existing mappings
				// If it exists, tnm[i] must match what it previously matched
				// If not, we bind this name to tnm[i]
				if(DEBUG) 
					cout << "matcher: free: " << rnm[i] << " match object: " << m->to_string() << endl;

				map<name,name> nmap = m->get_names();
				if(nmap.find(rnm[i]) == nmap.end()) {
					m->capture_name(rnm[i], tnm[i]);
					if(DEBUG) 
						cout << "matcher: free: new " << bigraph::name_to_string(rnm[i]) << " = " << bigraph::name_to_string(tnm[i]) << endl;
				} else {
					if(DEBUG) 
						cout << "matcher: free: old " << rnm[i] << endl;
					if(nmap[rnm[i]] != tnm[i]) return m->failure();
					if(DEBUG) 
						cout << "matcher: free: old matched " << rnm[i] << endl;
				}
			}
		}

		return try_match(t->get_suffix(), r->get_suffix(), m);
	} else {
		return m->failure();
	}
}

set<match *> matcher::try_match(parallel *t, prefix *r, match *m) {
	if(DEBUG) {
		rinfo("matcher::try_match") << "matching par: " << t->to_string() <<
			" against pref redex: " << r->to_string() << endl;
	}
	
	return m->failure();
}

bool is_compat(match *m1, match *m2) {
	if(m2->has_failed) return false;

	map<name,name> m1n = m1->get_names();
	map<name,name> m2n = m2->get_names();

	for(map<name,name>::iterator i = m1n.begin(); i != m1n.end(); i++) {
		if(m2n.find(i->first) == m2n.end()) continue;

		if(m2n[i->first] != i->second) return false;
	}

	map<term*,term*> mapping = m1->get_mappings();
	
	for(map<term*,term*>::iterator i = mapping.begin(); i!=mapping.end(); i++) {
		if(m2->get_mapping(i->first) == NULL) continue;

		if(m2->get_mapping(i->first) != i->second) return false;
	}

	return true;
}

set<match *> matcher::try_match(parallel *t, parallel *r, match *m) {
	// FIXME: This approach can be optimised considerably!
	if(DEBUG) {
		rinfo("matcher::try_match") << "matching: " << t->to_string() <<
			" against redex: " << r->to_string() << endl;
	}

	set<term *> tch = t->get_children();
	set<term *> rch = r->get_children();
	map<term *, set<match *> > matches;

	hole *has_hole = NULL; // Is there a top level hole?  e.g. A | B | $0
			 	// Something like A.$0 | B | C does not count.
				// This will be the hole term itself, or NULL.

	for(set<term *>::iterator i = rch.begin(); i != rch.end(); i++) {
		if((*i)->type == THOLE) {
			has_hole = (hole *)*i;
			rch.erase(i);
			break;
		}
	}

	if(has_hole == NULL && rch.size() > tch.size())
		return m->failure();

	if(m->root == NULL && !(t->parent == NULL || (t->parent != NULL && t->parent->active_context()))) {
		return m->failure();  // We can't start a new match here.
	} else if(m->root == NULL) {
		m->root = t;  // We can start a new match here!
	}
	
	m->add_match(r,t);

	for(set<term*>::iterator i = rch.begin(); i!= rch.end(); i++) {
		int mcount = 0;

		for(set<term*>::iterator j = tch.begin(); j != tch.end(); j++) {
			if(DEBUG)
				cout << "Matching: " << (*i)->to_string() << " against " << (*j)->to_string();
	
			match *mn = m->clone();
			set<match *> crossmatch = try_match(*j, *i, mn);

			if(crossmatch.size() > 0) {
				mcount++;
				matches[*i].insert(crossmatch.begin(), crossmatch.end());
				if(DEBUG)
					cout << ": matches " << crossmatch.size() << " times" << endl;
			} else {
				if(DEBUG) cout << ": no match" << endl;
			}
		}

		// We found nothing matching this part of the redex, so fail now.
		if(mcount == 0)
			return m->failure();
	}

	if(DEBUG) {
		cout << "PARALLEL MATCH:" << endl;

		for(map<term*,set<match *> >::iterator i = matches.begin(); i!=matches.end(); i++) {
			cout << "REDEX PART: " << i->first->to_string() << endl;
			cout << "MATCH SET: " << endl;

			for(set<match *>::iterator j = i->second.begin(); j!=i->second.end(); j++) {
				cout << (*j)->to_string() << endl;
			}
		}
	}

	set<match*> cand;
	for(set<term*>::iterator i = rch.begin(); i!=rch.end(); i++) {
		if(cand.size() == 0) {
			if(i != rch.begin()) return m->failure();

			cand = matches[*i];
			continue;
		}

		set<match *> ns = matches[*i];
		set<match *> newcand;

		for(set<match*>::iterator j = cand.begin(); j!=cand.end(); j++) {
			for(set<match*>::iterator k = ns.begin(); k!=ns.end(); k++) {
				if(is_compat(*j,*k)) {
					match *mm = (*j)->clone();
					mm->root = m->root;
					mm->incorporate(*k);
					newcand.insert(mm);
				}
			}
		}

		cand = newcand;
	}

	if(cand.size() == 0) return m->failure();

	// OK, now we have to go through and populate the parameter
	// with everything that was not matched.
	if(has_hole != NULL || t->parent == NULL) {
		if(t->parent == NULL && has_hole == NULL) {
			has_hole = new hole(999999);
		}

		for(set<match*>::iterator i = cand.begin(); i!=cand.end(); i++) {
			set<term*> ctx;

			for(set<term*>::iterator j = tch.begin(); j != tch.end(); j++) {
				if((*i)->get_mapping(*j) == NULL) {
					ctx.insert(*j);
				}
			}

			if(ctx.size() == 0) {
				(*i)->add_param(has_hole->index, new nil());
				continue;
			}

			if(ctx.size() == 1) {
				(*i)->add_param(has_hole->index, *(ctx.begin()));
				continue;
			}

			(*i)->add_param(has_hole->index, new parallel(ctx));
		}
	}

	if(DEBUG) {
		cout << "PARALLEL MATCH: RESULT:" << endl;

		for(set<match *>::iterator j = cand.begin(); j!=cand.end(); j++) {
			cout << (*j)->to_string() << endl;
		}
	}


	return cand;
}

/*
set<match *> matcher::try_match(parallel *t, parallel *r, match *m) {
	// FIXME: This approach can be optimised considerably!
	if(DEBUG) {
		rinfo("matcher::try_match") << "matching: " << t->to_string() <<
			" against redex: " << r->to_string() << endl;
	}

	set<term *> tch = t->get_children();
	set<term *> rch = r->get_children();
	map<term *, vector<set<match *> > > matches;

	hole *has_hole = NULL; // Is there a top level hole?  e.g. A | B | $0
			 	// Something like A.$0 | B | C does not count.
				// This will be the hole term itself, or NULL.

	for(set<term *>::iterator i = rch.begin(); i != rch.end(); i++) {
		if((*i)->type == THOLE) {
			has_hole = (hole *)*i;
			rch.erase(i);
			break;
		}
	}

	if(has_hole == NULL && rch.size() > tch.size())
		return m->failure();

	if(tch.size() == 0) return m->failure();

	if(m->root == NULL && !(t->parent == NULL || (t->parent != NULL && t->parent->active_context()))) {
		return m->failure();
	} else if(m->root == NULL) {
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

	set<string> checked;

	set<match *> res;

	do {
		// FIXME: This is a giant hack and is really gross, but it works this early in the morning.
		stringstream permstr;

		for(int i = 1; i<=ydim; i++) {
			permstr << cand[xdim-i];
		}

		if(checked.find(permstr.str()) != checked.end()) {
			continue; // We've already checked this permutation
		} else {
			checked.insert(permstr.str());
		}

		int k = 0;
		int succ = 0;
		
		match *nn = m->clone();

		for(set<term *>::iterator i = rch.begin(); i != rch.end(); i++) {
			if((*i)->type == THOLE) {
				rerror("matcher::try_match") << "A hole remains at the top-level in the redex" <<
					".  This is a bug.  Please report it." << endl;
				exit(1);
			}

			set<match*> mm = try_match(cand[xdim-1-k++],*i,nn);

			if(mm.size() == 0) {
				//delete nn;
				//mm.clear();
				nn->failure();
				break;
			}


			//res = match::merge(res,mm);		

			succ++;
		} 

		if(succ < ydim) {
			continue;
		}

		if(has_hole != NULL) {
			if(xdim - ydim > 1) {
				// There are "extra" things lying around, 
				// push these all into the hole.
				// The previous loop has matched cand[size - 1] ... cand[size - 1 - k]
				// elements and has incremented k once more.
				// Therefore we have cand[0] ... cand[size - k] elements to put in the hole.
				set<term*> nch;
			
				for(int l = 0; l<xdim-k; l++) {
					nch.insert(cand[l]);
					succ++;
				}

				parallel *np = new parallel(nch);
				nn->add_param(has_hole->index, np);
			} else if (xdim - ydim == 1) {
				nn->add_param(has_hole->index, cand[0]);
				succ++;
			} else {
				nn->add_param(has_hole->index, new nil());
			}
		}


		if(succ >= ydim) {
			res.insert(nn);
		} 
	} while(next_permutation(cand, cand+xdim));

	if(res.size() == 0) return m->failure();

	return res;
}
*/

bool no_overlap(list<match *> prev, match *cand) {
	// FIXME can probably optimise this with a LUT
	for(list<match *>::iterator i = prev.begin(); i!=prev.end(); i++) {
		if((*i)->root->overlap(cand->root) || cand->root->overlap((*i)->root)) {
			return false;
		}
	}

	return true;
}

set<match *> crossprod(set<match *>  m1, set<match *> m2) {
	// We have two sets {a,b,c} and {d,e,f}:
	// We want to construct:
	// {a.incorporate(d), a.incorporate(e), a.incorporate(f), b.inc...}

	if(DEBUG) cout << "crossprod(): " << m1.size() << " with " << m2.size() << endl;

	set<match *> res;

	for(set<match*>::iterator i = m1.begin(); i!=m1.end(); i++) {
		for(set<match*>::iterator j = m2.begin(); j!=m2.end(); j++) {
			assert((*i)->is_wide());

			if(!no_overlap(((wide_match *)(*i))->get_submatches(),*j))
				continue;

			wide_match *nm = (wide_match *)
				((wide_match *)(*i))->clone();
			nm->add_submatch(*j);
			res.insert(nm);
		}
	}

	m1.clear();
	m2.clear();
	
	if(DEBUG) cout << "crossprod(): res: " << res.size() << endl;

	return res;
}

set<match *> matcher::region_match(term *t, list<term *> redexes, set<match *> m) {
	if(redexes.size() == 0) {
		for(set<match*>::iterator i = m.begin(); i!=m.end(); i++)
			(*i)->success();

		return m;
	}

	term *redex = redexes.front();
	redexes.pop_front();

	set<match *> result;

	for(set<match *>::iterator i = m.begin(); i!=m.end(); i++) {
		// For each existing wide match, we get a new set of matches:
		set<match*> ms = try_match_anywhere(t, redex, (*i)->get_rule(), *i);

		if(ms.size() == 0)
			(*i)->failure();

		set<match*> cp = crossprod(match::singleton(*i),ms);

		result.insert(cp.begin(),cp.end());
	}

	return region_match(t,redexes,result);
}

set<match *> matcher::try_match(term *t, regions *r, match *m) {
	if(DEBUG) {
		rinfo("matcher::try_match") << "matching region: " << t->to_string() <<
			" against redex: " << r->to_string() << endl;
	}

	set<match *> matches;
	list<term *> ch = r->get_children();

	if(m->root != NULL || t->parent != NULL) 
		return m->failure();

	m->root = t;
	m->add_match(r,t);

	match *wm = new wide_match(m->get_rule());

	return region_match(t,ch,match::singleton(wm));
}

set<match *> matcher::try_match(prefix *t, parallel *r, match *m) {
	// This covers cases like redex: a.(b | $0) matching against a.b where $0 = nil

	set<term *> ch = r->get_children();

	if(ch.size() > 2) return m->failure();
	
	hole *has_hole = NULL;
	term *non_hole = NULL;

	for(set<term *>::iterator i = ch.begin(); i != ch.end(); i++) {
		if((*i)->type == THOLE) {
			if(has_hole != NULL) return m->failure();
			has_hole = (hole *)*i;
		} else {
			if(non_hole != NULL) return m->failure();
			non_hole = *i;
		}
	}

	if(non_hole == NULL || has_hole == NULL) return m->failure();

	m->add_param(has_hole->index, new nil());

	return try_match(t,non_hole,m);
}

set<match *> matcher::try_match(term *t, hole *r, match *m) {
	m->success();
	m->add_param(r->index, t);
	return match::singleton(m);
}

set<match *> matcher::try_match(num *t, num *r, match *m) {
	if(t->value == r->value) {
		if(m->root == NULL) {
			m->root = t;
		}

		m->success();
		return match::singleton(m);
	}

	return m->failure();
}

set<match *> matcher::try_match(nil *t, term *r, match *m) {
	if(r->type == TNIL) {
		m->add_match(r,t);
		m->success();
		return match::singleton(m);
	} else if (r->type == THOLE) {
		m->success();
		m->add_param(((hole *)r)->index, t);
		return match::singleton(m);
	}

	return m->failure();
}

set<match *> matcher::try_match(term *t, term *r, match *m) {
	if(t->type == TPREF && r->type == TPREF)
		return try_match((prefix*)t, (prefix*)r, m);
	if(t->type == TPAR && r->type == TPREF)
		return try_match((parallel*)t, (prefix*)r, m);
	if(t->type == TPREF && r->type == TPAR) 
		return try_match((prefix*)t, (parallel*)r, m);
	if(t->type == TPAR && r->type == TPAR)
		return try_match((parallel*)t, (parallel*)r, m);
	if(t->type == TNUM && r->type == TNUM)
		return try_match((num*)t, (num*)r, m);
	if(r->type == TREGION)
		return try_match(t, (regions*)r, m);
	if(r->type == THOLE)
		return try_match(t, (hole*)r, m);
	if(t->type == TNIL && r->type == TNIL)
		return try_match((nil*)t, (nil*)r, m);

	return m->failure();
}

set <match *> matcher::try_match(term *t, reactionrule *r) {
	set<match *> matches;

	if(r->redex->type == TREGION)
		return try_match(t, r->redex, new match(r));

	term *p = t->next();
	while(p != NULL) {
		if(p->parent == NULL || p->parent->active_context()) {
			match *nm = new match(r);
			matches = match::merge(matches, try_match(p, r->redex, nm));
		}
		p = t->next();
	}

	t->reset();

	return matches;
}

set <match *> matcher::try_match_anywhere(term *t, term *r, reactionrule *rl, match *m) {
	set<match *> matches;

	if(DEBUG)
		cout << "matcher::try_match_anywhere: " << m->to_string() << endl;

	term *p = t->next();
	while(p != NULL) {
		if(DEBUG) cout << "p: " << p->to_string() << ": ";
		if(p->parent == NULL || p->parent->active_context()) {
			if(DEBUG) cout << "active" << endl;
			match *nm = new match(rl);
			nm->incorporate(m);
			if(m->parent != NULL) nm->incorporate(m->parent);
			//cout << "matcher::try_match_anywhere: nm: " << nm->to_string() << endl;
			matches = match::merge(matches, try_match(p, r, nm));
		} else {
			if(DEBUG) cout << "passive" << endl;
		}

		p = t->next();
	}

	t->reset();

	return matches;
}



