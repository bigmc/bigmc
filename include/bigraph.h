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
#ifndef _BIGRAPH_H
#define _BIGRAPH_H

// Bigraph is (V,E,ctrl,prnt,link) : <n,K> -> <m,L>
class bigraph {
	term *root;
	int nholes;
	int nregions;
	set<name> inner;
	set<name> outer;
	set<reactionrule *> rules;
public:
	static map<string,name> name_map;
	static map<string,control> control_map;
	static map<control,bool> activity_map;
	static map<control,int> arity_map;
	static set<name> names;
	static control u_control;
	static name u_name;
	bigraph(int roots);
	~bigraph();
	term *get_root(int n);
	bigraph operator + (bigraph b);
	set<match *> find_matches();
	long hash();
	static control control_from_string(string n);
	static string control_to_string(control c);
	static name name_from_string(string n);
	static string name_to_string(name n);
	static bool activity(control c);
	static int arity(control c);
	static control add_control(string n, bool act, int ar);
	void add_outer_name(name n);
	void add_inner_name(name n);
	set<reactionrule*> get_rules();
	void set_root(term *n);
	void add_rule(reactionrule *r);
	static bool is_free(name n);
	bigraph *apply_match(match *m);
	string to_string();
};


#endif
