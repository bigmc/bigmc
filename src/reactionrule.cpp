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
#include <map>
#include <iostream>
#include <assert.h>

#include <bigmc.h>

int reactionrule::u_ctx = 0;

reactionrule::reactionrule() {
	assert(0);
}

reactionrule::reactionrule(term *red, term *reac) {
	redex = red;
	reactum = reac;
	name = "";
}

reactionrule::~reactionrule() {

}

string reactionrule::to_string() {
	assert(this != NULL);

	if(name != "") return name;

	if(redex != NULL && reactum != NULL)
		return redex->to_string() + " -> " + reactum->to_string();
	if(redex != NULL)
		return redex->to_string() + " -> NULL";
	if(reactum != NULL)
		return "NULL -> " + reactum->to_string();
	return "<error printing rule>";
}

void reactionrule::contextify() {
	if(redex->type != TPAR) return;

	bool hf = false;

	set<term *> ch = ((parallel *)redex)->get_children();

	for(set<term*>::iterator i = ch.begin(); i!=ch.end(); i++) {
		if((*i)->type == THOLE) return;
	}

	ch.insert(new hole(19191911+u_ctx));

	// FIXME: possibly leaking the old redex here
	redex = new parallel(ch);

	if(reactum->type == TPAR) {
		set<term *> ch2 = ((parallel *)reactum)->get_children();
		ch2.insert(new hole(19191911+u_ctx));
		reactum = new parallel(ch2);
	} else {
		set<term *> ch2;
		ch2.insert(reactum);
		ch2.insert(new hole(19191911+u_ctx));
		reactum = new parallel(ch2);
	}

	u_ctx++;
}

void reactionrule::set_name(string n) {
	name = n;
}

string reactionrule::get_name() {
	return name;
}

set<reactionrule *> reactionrule::get_causation() {
	return causation;
}

void reactionrule::causes(reactionrule *r) {
	causation.insert(r);
}
