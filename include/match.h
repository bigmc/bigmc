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
protected:
	map<name,name> names;
	map<int,term *> parameters;
	map<term *,term *> mapping;
	reactionrule *rule;
public:
	match(reactionrule *rl);
	~match();
	void add_param(int id, term *c);
	term *get_param(int id);
	void add_match(term *src, term *target);
	term *get_mapping(term *src);
	map<term*,term*> get_mappings();
	void capture_name(name src, name target);
	map<name,name> get_names();
	name get_name(name n);
	reactionrule *get_rule();
	void set_rule(reactionrule *r);
	term *root;
	//term *next();
	set<match *> failure();
	void success();
	static set<match *> singleton(match *t);	
	list<term *> remaining();
	virtual match *clone();
	match *fresh(term *head, list<term *> rem);
	void advance(term *head, list<term *> rem);
	static set<match *> merge(set<match *> a, set<match *> b);
	virtual string to_string();
	bool has_succeeded;
	bool has_failed;
	void incorporate(match *other);
	virtual const bool is_wide();
	match *parent;

};

class wide_match : public match {
	list<match *> submatches;
public:
	wide_match(reactionrule *r);
	~wide_match();
	void add_submatch(match *m);
	list<match *> get_submatches();
	match *clone();
	const bool is_wide();
	string to_string();
};

#endif

