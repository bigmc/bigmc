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
#ifndef _SUBTREE_H
#define _SUBTREE_H

#include <string>
#include <sstream>
#include <deque>
#include <vector>

class st_el {
public:
	st_el(term *t, string n);
	~st_el();
	term *data;
	string name;
	bool operator <(st_el &l);
};

class subtree {
public:
	static list<st_el*> preorder_string(term *t);
	static list<st_el*> preorder_string(prefix *t);
	static list<st_el*> preorder_string(parallel *t);
	static list<st_el*> preorder_string(nil *t);
	static list<st_el*> preorder_string(hole *t);
};

#endif
