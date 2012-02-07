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
#include <bigraph.h>
#include <reactionrule.h>


void prefix::init(unsigned long sid, control c, vector<name> ports, term *suff) {
	ctrl = c;
	arity = bigraph::arity(c);
	active = bigraph::activity(c);

	port = ports;
	suffix = suff;

	if(suffix)
		suffix->parent = this;

	type = TPREF;

	if(port.size() > arity) {
		cerr << "Error: control " << bigraph::control_to_string(c) << " has arity " << arity << " but " << port.size() << " ports have been linked!" << endl;
		exit(1);
	}

	id = sid;
}

prefix::prefix(unsigned long sid, control c, vector<name> ports, term *suff) {
	init(sid,c,ports,suff);
}

prefix::prefix(control c, vector<name> ports, term *suff) {
	init(term::u_term++, c, ports, suff);
}

prefix::~prefix() {
	delete suffix;
}

string prefix::to_string() {
	string nm = "";

	if(port.size() == 0)
		nm = "";
	else {
		for(int i = 0; i<port.size(); i++) {
			if(port[i] == 0) {
				nm += "-";
			} else {
				nm += bigraph::name_to_string(port[i]);
			}
			if(i != port.size()-1)
				nm += ",";
		}

		if(nm != "") nm = "[" + nm + "]";

		
	}

	if(DEBUG) {
		stringstream out;
		out << "@" << id;
		nm = out.str();
	}

	if(suffix != NULL)
		return bigraph::control_to_string(ctrl) + nm + "." + suffix->to_string();
	else
		return bigraph::control_to_string(ctrl) + nm + ".NULL";
}

control prefix::get_control() {
	return ctrl;
}

term *prefix::get_suffix() {
	return suffix;
}

set<match *> prefix::find_matches(match *m) {
	if(DEBUG) cout << "BUG: prefix::find_matches()" << endl;

	if(m->has_succeeded || m->has_failed) return set<match *>();

	list<term *> r = m->remaining();

	if(r.size() == 0 || (r.size() == 1 && r.front()->type == TNIL)) {
		return m->failure();
	}

	if(r.size() == 1) {
		term *t = r.front();

		if(t->type == TPREF) {
			if(DEBUG) cout << "BUG: prefix::find_matches(): redex front: " << t->to_string() << " matching against: " << to_string() << endl;
			prefix *tp = dynamic_cast<prefix *>(t);
			assert(tp != NULL);

			if(tp->ctrl == ctrl) {
				if(DEBUG) cout << "BUG: prefix::find_matches(): match found" << endl;
				// We have a match!

				if(m->root == NULL && !active_context())
					return m->failure();

				// If this is a new match, set the root to here.
				if(m->root == NULL)
					m->root = this;
				
				m->add_match(t,this);

				// Now we send this match on its merry way into the suffix:
				m->advance(suffix, term::singleton(tp->suffix));
				
				return suffix->find_matches(m);

				// And we need to create a new match that could potentially start
				// here! 

				//match *newmatch = m->fresh(NULL, term::singleton(m->get_rule()->redex));
				
				//return match::merge(suffm,suffix->find_matches(newmatch));
				//return suffm;
			} else {
				if(DEBUG) cout << "BUG: prefix::find_matches(): match not found" << endl;
				//match *newmatch = m->fresh(NULL, term::singleton(m->get_rule()->redex));
				//return suffix->find_matches(newmatch);
				return m->failure();
			}

		} else if(t->type == THOLE) {
			if(m->root == NULL && !active_context())
				return m->failure();

			// A hole will always match a prefix.
			if(m->root == NULL)
				m->root = this;
			m->success();
			m->add_param(((hole *)t)->index, this);
			m->add_match(t,this);
			m->advance(NULL,list<term*>());
			return match::singleton(m);
		} else {
			return m->failure();
		}
	}

	return m->failure();
} 

term *prefix::apply_match(match *m) {
	if(id == m->root->id) {
		// This is the match site.  Return the reactum.
		if(DEBUG) cout << "BUG: prefix::apply_match(): Found match site!" << endl;
		term *r = m->get_rule()->reactum->instantiate(m);
		if(DEBUG) cout << "Returning reactum: " << r->to_string() << endl;
		return r;
	} else {
		if(DEBUG) cout << "BUG: prefix::apply_match(): not match site: " << this << " != " << m->root << endl;
		return new prefix(id,ctrl,port,suffix->apply_match(m));
	}
}

term *prefix::instantiate(match *m) {
	set<term *> n;

	if(m == NULL) {
		if(DEBUG) cout << "prefix::instantiate(NULL): " << to_string() << endl;
		return new prefix(term::u_term++,ctrl,port,suffix->instantiate(m));
	}

	vector<name> nport (port.size());

	for(int i = 0; i<port.size(); i++) {
		nport[i] = m->get_name(port[i]);
		if(DEBUG)
			cout << "prefix::instantiate: nport[" << i << "] = getname(" << port[i] << ") == " << nport[i] << endl;
	}
	
	if(DEBUG) cout << "prefix::instantiate(!NULL): " << to_string() << endl;

	return new prefix(term::u_term++,ctrl,nport,suffix->instantiate(m));
}

unsigned int prefix::size() {
	return 1 + suffix->size();
}

void prefix::accept(termvisitor *t) {
	suffix->accept(t);
	t->visit(this);
}

bool prefix::active_context() {
	if(parent == NULL) return active;

	return active && parent->active_context();
}

vector<name> prefix::get_ports() {
	return port;
}

