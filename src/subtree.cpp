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
#include <subtree.h>

/* An implementation of Luccio et al.'s sublinear exact tree matching, as described in
 "Exact rooted subtree matching in sublinear time". */


bool cmp_st_elp (st_el *a, st_el *b) {
	if(a->name == "[-]") return false;
	if(b->name == "[-]") return true;

	return a->name < b->name;
}

list<st_el*> subtree::preorder_string(term *t) {
	switch(t->type) {
		case TPREF: return preorder_string((prefix *)t);
		case TPAR:  return preorder_string((parallel *)t);
		case THOLE: return preorder_string((hole *)t);
		case TNIL:  return preorder_string((nil *)t);
		case TNUM:  return preorder_string((num *)t);
		default: return list<st_el*>();
	}
}

list<st_el*> subtree::preorder_string(prefix *t) {
	st_el* e = new st_el(t, bigraph::control_to_string(t->get_control()) + fports(t->get_ports()));

	list<st_el*> tl = preorder_string(t->get_suffix());

	tl.push_front(e);
	tl.push_back(new st_el(NULL, "0"));

	return tl;
}

list<st_el*> subtree::preorder_string(nil *t) {
	list<st_el*> l;

	//l.push_back(new st_el(NULL, "0"));

	return l;
}

list<st_el*> subtree::preorder_string(parallel *t) {
	set<term *> ch = t->get_children();
	list<st_el*> sorted_ch;
	list<st_el*> res;

	for(set<term *>::iterator i = ch.begin(); i!=ch.end(); i++) {
		switch((*i)->type) {
		case TPREF:
			sorted_ch.push_back(
				new st_el(*i,bigraph::control_to_string(
					((prefix *)(*i))->get_control()) + fports(((prefix *)(*i))->get_ports())));
			break;
		case THOLE:
			sorted_ch.push_back(new st_el(*i, "[-]"));
			break;
		case TNUM:
			sorted_ch.push_back(new st_el(*i, (*i)->to_string()));
			break;
		case TNIL:
			break;
		}
	}

	sorted_ch.sort(cmp_st_elp);

	for(list<st_el*>::iterator i = sorted_ch.begin(); i!=sorted_ch.end(); i++) {
		list<st_el*> j = preorder_string((*i)->data);
		res.insert(res.end(), j.begin(), j.end());
	}


	return res;
}

list<st_el*> subtree::preorder_string(hole *t) {
	list<st_el*> l;
	l.push_back(new st_el(t, "[-]"));
	return l;
}

list<st_el*> subtree::preorder_string(num *t) {
	list<st_el*> l;
	l.push_back(new st_el(t, t->to_string()));
	return l;
}

string subtree::fports(vector<name> prt) {
	if(prt.size() == 0) return "[]";

	string res = "[";
	int i = 0;
	res += bigraph::name_to_string(prt[i]);


	for(i = 1;i<prt.size();i++) {
		res += "," + bigraph::name_to_string(prt[i]);
	}

	return res + "]";
}

string subtree::ordered_string(term *t) {
	list<st_el*> l = preorder_string(t);

	if(l.size() == 0) return "";

	string res = l.front()->name;
	l.pop_front();

	for(list<st_el*>::iterator i = l.begin(); i != l.end(); i++) {
		res += " " + (*i)->name;
		delete *i;
	}

	return res;
}

st_el::st_el(term *t, string n) {
	data = t;
	name = n;
}

st_el::~st_el() {

}

bool st_el::operator< (st_el &other) {
	return name < other.name;
}

