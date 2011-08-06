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
#include <assert.h>

#include <bigmc.h>

query::query() {
	;
}

query::~query() {

}

string query::to_string() {
	return "<uninitialised query>";
}

bool query::check(node *n) {
	assert(0);
	return false;
}

int query::eval(node *n) {
	rerror("query::eval") << "Query object '" << to_string() << "' is of the wrong type:" << 
		" expected 'int', got 'bool'" << endl;
	exit(1);
	return 0;
}

// BINARY OP

query_bin::query_bin(query *l, int opr, query *r) {
	lhs = l;
	rhs = r;
	oper = opr;
}

query_bin::~query_bin() {

}

string query_bin::to_string() {
	string op = "";

	switch(oper) {
		case OPR_NEQ:
			op = "!=";
			break;
		case OPR_EQ:
			op = "==";
			break;
		case OPR_LEQ:
			op = "<=";
			break;
		case OPR_GEQ:
			op = ">=";
			break;
		case OPR_GT:
			op = ">";
			break;
		case OPR_LT:
			op = "<";
			break;
		case OPR_AND:
			op = "&&";
			break;
		case OPR_OR:
			op = "||";
			break;
		default:
			op = "??";
			break;
	}

	return lhs->to_string() + " " + op + " " + rhs->to_string();
}

bool query_bin::check(node *n) {
	switch(oper) {
		case OPR_NEQ:
			return lhs->eval(n) != rhs->eval(n);
		case OPR_EQ:
			return lhs->eval(n) == rhs->eval(n);
		case OPR_LEQ:
			return lhs->eval(n) <= rhs->eval(n);
		case OPR_GEQ:
			return lhs->eval(n) >= rhs->eval(n);
		case OPR_GT:
			return lhs->eval(n) > rhs->eval(n);
		case OPR_LT:
			return lhs->eval(n) < rhs->eval(n);
		case OPR_AND:
			return lhs->check(n) && rhs->check(n);
		case OPR_OR:
			return lhs->check(n) || rhs->check(n);
		default:
			break;
	}

	rerror("query_bin::check") << "unhandled operator: " << to_string() << endl;
	exit(1);

	return false;
}

// NOT 

query_not::query_not(query *l) {
	lhs = l;
}

query_not::~query_not() {

}

string query_not::to_string() {
	return "!" + lhs->to_string();
}

bool query_not::check(node *n) {
	// TODO: Emit diagnostic infnotmation here in event of failure
	return !lhs->check(n);
}

// PREDICATE 

query_predicate::query_predicate(string nm, list<query_val *> p) {
	name = nm;
	params = p;
	pred = predicate::get_predicate(nm);
	assert(pred != NULL);
}

query_predicate::~query_predicate() {

}

string query_predicate::to_string() {
	string s = name;
	if(params.size() == 0)
		return s + "()";
	else
		return s + "(...)";

		//return s + "(" + param->to_string() + ")";
}

bool query_predicate::check(node *n) {
	return pred->check(n,params);
}

int query_predicate::eval(node *n) {
	return pred->eval(n,params);
}

void query_predicate::cleanup() {

}

// query_val
query_val::query_val() {
}

query_val::~query_val() {

}

string query_val::to_string() {
	return "<uninitialised query_val>";
}

bool query_val::check(node *n) {
	rerror("query_val::check") << "hit invalid node in check()" << endl;
	exit(1);
	return false;
}

int query_val::eval(node *n) {
	rerror("query_val::eval") << "hit invalid node in eval()" << endl;
	exit(1);
	return 0;

}


// query_num

query_num::query_num(int d) {
	value = d;
}

query_num::~query_num() {

}

string query_num::to_string() {
	return "<int literal>";
}

bool query_num::check(node *n) {
	rwarning("query_num::check") << "coercing int to bool" << endl;
	return value > 0;
}


int query_num::eval(node *n) {
	return value;
}

// query_term

query_term::query_term(term *d) {
	data = d;
}

query_term::~query_term() {

}

string query_term::to_string() {
	return data->to_string();
}

bool query_term::check(node *n) {
	rerror("query_term::check") << "unexpected term in property" << endl;
	exit(1);
	return false;
}


int query_term::eval(node *n) {
	rerror("query_term::eval") << "Type error: expected 'int', got 'term'" << endl;
	exit(1);
	return 0;

}

// query_id

query_id::query_id(string id) {
	name = id;
}

query_id::~query_id() {

}

string query_id::to_string() {
	return "$" + name;
}

bool query_id::check(node *n) {
	if(name == "true") return true;
	if(name == "false") return false;

	rerror("query_id::check") << "unknown id '" << name << "' in property" << endl;
	exit(1);
	return false;
}


int query_id::eval(node *n) {
	rerror("query_id::eval") << "invalid integer id in eval()" << endl;
	exit(1);
	return 0;

}

// query_scope

query_scope::query_scope(string nm, query *q) {
	name = nm;
	qry = q;
}

query_scope::~query_scope() {

}

string query_scope::to_string() {
	return "$" + name + "->" + qry->to_string();
}

bool query_scope::check(node *n) {
	if(name == "this") {
		return qry->check(n);
	} else if(name == "terminal") {
		if(n->terminal) return qry->check(n);
		return true;
	} else if (name == "pred") {
		if(n->parent == NULL) return true;
		return qry->check(n->parent);
	} else if (name == "succ") {
		for(set<pair<node *, reactionrule *> >::iterator i = n->target.begin();
			i != n->target.end(); i++) {
			if(!qry->check(i->first)) return false;
		}

		return true;
	}

	rerror("query_scope::check") << "Unknown meta-variable $" << name << endl;
	exit(1);
	return false;
}

int query_scope::eval(node *n) {
	// FIXME: this probably needs to deal with sets of integers, not just ints
	if(name == "this") {
		return qry->eval(n);
	} else if(name == "terminal") {
		if(n->terminal) return qry->eval(n);
		rwarning("query_scope::eval") << "using $terminal doesn't make much sense" 
					<< " in this context.  Returning 0" << endl;
		return 0;
	} else if (name == "pred") {
		if(n->parent == NULL) return 0;
		return qry->eval(n->parent);
	}

	rerror("query_scope::eval") << "Unknown meta-variable $" << name << endl;
	exit(1);
	return false;

}



