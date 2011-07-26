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

string subtree::preorder_string(term *t) {
	switch(t->type) {
		case TPREF: return preorder_string((prefix *)t);
		case TPAR:  return preorder_string((parallel *)t);
		case THOLE: return preorder_string((hole *)t);
		case TNIL:  return preorder_string((nil *)t);
		default: return "<unhandled>";
	}
}

string subtree::preorder_string(prefix *t) {
	return " " + bigraph::control_to_string(t->get_control()) +
		 preorder_string(t->get_suffix());
}

string subtree::preorder_string(nil *t) {
	return " 0";
}

string subtree::preorder_string(parallel *t) {
	string s = " ";

	set<term *> ch = t->get_children();
	
	for(set<term *>::iterator i = ch.begin(); i!=ch.end(); i++) {
		s += preorder_string(*i);
	}

	return s + " 0";
}

string subtree::preorder_string(hole *t) {
	return " [-]";
}
