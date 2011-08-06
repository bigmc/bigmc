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
#ifndef _QUERY_H
#define _QUERY_H

class query {
public:
	query();
	~query();
	virtual string to_string();
	virtual bool check(node *n);
	virtual int eval(node *n);
};

class query_val : public query {
public:
	query_val();
	~query_val();
	virtual string to_string();
	virtual bool check(node *n);
	virtual int eval(node *n);
};

class query_num : public query_val {
public:
	query_num(int d);
	~query_num();
	int value;
	string to_string();
	bool check(node *n);
	int eval(node *n);
};

class query_term : public query_val {
public:
	query_term(term *t);
	~query_term();
	term *data;
	string to_string();
	bool check(node *n);
	int eval(node *n);
};

class query_id : public query_val {
public:
	query_id(string id);
	~query_id();
	string name;
	string to_string();
	bool check(node *n);
	int eval(node *n);
};

class query_bin : public query {
public:
	query *lhs;
	query *rhs;
	int oper;
	query_bin(query *l, int opr, query *r);
	~query_bin();
	string to_string();
	bool check(node *n);
};

class query_not : public query {
public:
	query *lhs;
	query_not(query *l);
	~query_not();
	string to_string();
	bool check(node *n);
};


class query_predicate : public query {
	predicate *pred;
public:
	list<query_val *> params;
	string name;
	query_predicate(string nm, list<query_val*> l);
	~query_predicate();
	string to_string();
	bool check(node *n);
	int eval(node *n);
	static void cleanup();
};

class query_scope : public query {
public:
	query_scope(string nm, query *q);
	~query_scope();
	string name;
	query *qry;
	string to_string();
	bool check(node *n);
	int eval(node *n);
};



#endif
