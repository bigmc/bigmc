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
#ifndef _REACTIONRULE_H
#define _REACTIONRULE_H

class reactionrule {
	static int u_ctx;
	string name;
	set<reactionrule *> causation;
public:
	term *redex;
	term *reactum;
	reactionrule();
	reactionrule(term *red, term *reac);
	~reactionrule();
	string to_string();
	void contextify();
	void set_name(string n);
	string get_name();
	set<reactionrule *> get_causation();
	void causes(reactionrule *r);
};

#endif

