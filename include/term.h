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
#ifndef _TERM_H
#define _TERM_H

#include <string>
#include <sstream>
#include <deque>
#include <vector>

#define TPREF	1
#define TPAR	2
#define THOLE	4
#define TNIL	8
#define TREGION 16
#define TNUM	32

class termvisitor;

class term {
protected:
	deque<term *> remaining;
public:
	term();
	virtual ~term();
	virtual string to_string();
	int type;
	virtual set<match *> find_matches(match *m);
	virtual term *apply_match(match *m);
	virtual term *instantiate(match *m);
	static list<term *> singleton(term *t);
	term *next();
	void reset();
	static set<match *> find_all_matches(term *t, reactionrule *m);
	static bool matches(term *subj, term *redex);
	virtual unsigned int size();
	virtual void accept(termvisitor *t);
	term *parent;
	virtual bool active_context();
	virtual set<term*> flatten();
	bool overlap(term *other);
	unsigned long id;
	static unsigned long u_term;
};

class parallel : public term {
	set<term *> terms;
	void init(unsigned long sid, set<term *> l);
public:
	parallel(set<term *> l);
	parallel(unsigned long sid, set<term *> l);
	virtual ~parallel();
	string to_string();
	set<term *> get_children();
	set<match *> find_matches(match *m);
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);
	set<term*> flatten();
	bool active_context();
};

class regions : public term {
	list<term *> terms;
	void init(unsigned long sid, list<term *> l);
public:
	regions(list<term *> l);
	regions(unsigned long sid, list<term *> l);
	virtual ~regions();
	string to_string();
	list<term *> get_children();
	set<match *> find_matches(match *m);
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);
	set<term*> flatten();
};

class prefix : public term {
	control ctrl;
	int arity;
	static int u_term;
	vector<name> port;
	term *suffix;
	void init(unsigned long sid, control ctrl, vector<name> ports, term *suff);
public:
	prefix(control ctrl, vector<name> ports, term *suff);
	prefix(unsigned long sid, control ctrl, vector<name> ports, term *suff);
	virtual ~prefix();
	control get_control();
	string to_string();
	term *get_suffix();
	set<match *> find_matches(match *m);
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);
	bool active_context();
	vector<name> get_ports();
	bool active;
};

class hole : public term {
public:
	hole(int idx);
	virtual ~hole();
	string to_string();
	set<match *> find_matches(match *m);
	int index;
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);

};

class num : public term {
public:
	num(int val);
	virtual ~num();
	string to_string();
	set<match *> find_matches(match *m);
	int value;
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);

};


class nil : public term {
public:
	nil();
	virtual ~nil();
	string to_string();
	set<match *> find_matches(match *m);
	term *apply_match(match *m);
	term *instantiate(match *m);
	unsigned int size();
	void accept(termvisitor *t);

};

class termvisitor {
public:
	virtual void visit(term *t);
	virtual void visit(parallel *t);
	virtual void visit(regions *t);
	virtual void visit(prefix *t);
	virtual void visit(hole *t);
	virtual void visit(nil *t);
	virtual void visit(num *t);
};

class termconsistencyvisitor : public termvisitor {
	set<term *> visited;
public:
	void visit(term *t);
	void visit(parallel *t);
	void visit(regions *t);
	void visit(prefix *t);
	void visit(hole *t);
	void visit(nil *t);
	void visit(num *t);
};

#endif


