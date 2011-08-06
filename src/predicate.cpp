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
#include <map>

#include <assert.h>
#include <bigmc.h>

map<string,predicate*> predicate::predicates;

predicate::predicate() {

}

predicate::~predicate() {
}

bool predicate::check(node *n,list<query_val *> param) {
	assert(0);
	return false;
}


int predicate::eval(node *n,list<query_val *> param) {
	assert(0);
	return false;
}

void predicate::register_predicate(string name, predicate *p) {
	predicates[name] = p;
}

predicate *predicate::get_predicate(string name) {
	if(predicates.find(name) == predicates.end()) return NULL;

	return predicates[name];
}
