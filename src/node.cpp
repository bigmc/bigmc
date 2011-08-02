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
#include <assert.h>
#include <bigmc.h>

node::node(bigraph *b, node *psrc, reactionrule *r) {
	parent = psrc;
	bg = b;
	visited = false;
	terminal = false;
	rule = r;

	// FIXME: This is a kludge
        
	// sdbm hash over the to_string() of the bigraph (!!!!!!!!!!!)
	hash = 0;
        int c;
	string s = subtree::ordered_string(b->get_root(0));

	int i = 0;
        while ((c = s[i++]) != '\0')
            hash = c + (hash << 6) + (hash << 16) - hash;
}

node::~node() {
	delete bg;
}

void node::add_target(node *n, reactionrule *r) {
	target.insert(pair<node*,reactionrule*>(n,r));
}

void node::set_visited(bool v) {
	visited = v;
}

bool node::is_visited() {
	return visited;
}

