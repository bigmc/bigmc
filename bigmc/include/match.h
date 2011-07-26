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
#ifndef _MATCH_H
#define _MATCH_H

#include <deque>
#include <list>
#include <map>

class term;
class reactionrule;

class match {
	map<int,term *> parameters;
	map<term *,term *> mapping;
	reactionrule *rule;
	list<term *> redex;
	term *matchhead;
	match *parent;
	long foobar;
public:
	match(term *head, list<term *>rem, match *parent, reactionrule *rl);
	~match();
	void add_param(int id, term *c);
	term *get_param(int id);
	void add_match(term *src, term *target);
	term *get_mapping(term *src);
	reactionrule *get_rule();
	term *root;
	//term *next();
	set<match *> failure();
	void success();
	static set<match *> singleton(match *t);	
	list<term *> remaining();
	match *clone(term *head, list<term *> rem);
	match *fresh(term *head, list<term *> rem);
	void advance(term *head, list<term *> rem);
	static set<match *> merge(set<match *> a, set<match *> b);
	string to_string();
	bool has_succeeded;
	bool has_failed;
	void incorporate(match *other);

};

#endif

