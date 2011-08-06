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
#include <bigmc.h>
#include <bigraph.h>
#include <node.h>
#include <iostream>
#include <list>

pred_matches::pred_matches() {

}

pred_matches::~pred_matches() {

}

bool pred_matches::check(node *n, list<query_val *> param) {
	return eval(n,param) > 0;
}

int pred_matches::eval(node *n, list<query_val *> param) {
	if(param.size() == 0 || param.size() > 1) {
		PRED_FAIL_PARAM("matches", "term -> int", "_ -> int");
		exit(1);
	}

	query_term *t = dynamic_cast<query_term *>(param.front());

	if(t == NULL) {
		PRED_FAIL_PARAM("matches", "term -> int", "??? -> int");
		exit(1);
	}

	reactionrule *r = new reactionrule(t->data, NULL);
	match *mt = new match(r);

	set<match *> m = matcher::try_match_anywhere(n->bg->get_root(0), t->data, r, mt);

	int sz = m.size();

	mt->failure();
	delete r;
	
	for(set<match *>::iterator i = m.begin(); i!=m.end(); i++) {
		(*i)->failure();
	}

	return sz;
}
