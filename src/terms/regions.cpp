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
#include <assert.h>
#include <bigmc.h>

regions::regions(list<term *>l) {
	init(term::u_term++,l);
}
regions::regions(unsigned long sid, list<term *>l) {
	init(sid,l);
}

void regions::init(unsigned long sid, list<term *>l) {
	terms = l;
	type = TREGION;
	id = sid;

	for(list<term *>::iterator i = l.begin(); i!=l.end(); i++) {
		(*i)->parent = this;
	}
}

regions::~regions() {
	list<term*>::iterator i = terms.begin();
	while(i != terms.end()) {
		delete *i;
		i++;
	}

	terms.clear();
}

string regions::to_string() {
	string s = "";

	unsigned int k = 0;
	for(list<term *>::iterator i = terms.begin(); i!=terms.end(); ++i) {
		k++;

		s += (*i)->to_string();

		if(k < terms.size())
			s += " || ";
	}

	return s;
}

set<match *> regions::find_matches(match *m) {
	return m->failure();
} 

list<term *> regions::get_children() {
	return terms;
}

term *regions::apply_match(match *m) {
	if(id == m->root->id) {
		// This is the match site.  Return the reactum.
		if(DEBUG) cout << "BUG: regions::apply_match(): Found match site!" << endl;
		term *r = m->get_rule()->reactum->instantiate(m);
		if(DEBUG) cout << "Returning reactum: " << r->to_string() << endl;
		return r;
	} 

	list<term *> n;

	for(list<term*>::iterator i = terms.begin(); i!=terms.end(); i++) {
		n.push_back((*i)->apply_match(m));
	}

	regions *pp = new regions(id,n);
	pp->flatten();

	return pp;
}

term *regions::instantiate(match *m) {
	list<term *> n;

	for(list<term*>::iterator i = terms.begin(); i!=terms.end(); i++) {
		n.push_back((*i)->instantiate(m));
	}

	regions *pp = new regions(term::u_term++,n);
	return pp;
}

set<term*> regions::flatten() {
	set<term *> s;
	s.insert(this);
	return s;
}



unsigned int regions::size() {
	unsigned int sz = 0;
	for(list<term *>::iterator i = terms.begin(); i!=terms.end(); i++) {
		sz += (*i)->size();
	}

	return sz;
}

void regions::accept(termvisitor *t) {
	for(list<term*>::iterator i = terms.begin(); i != terms.end(); i++) {
		(*i)->accept(t);
	}

	t->visit(this);
}

