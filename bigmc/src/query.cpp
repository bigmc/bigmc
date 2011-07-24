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
			break;
		case OPR_EQ:
			break;
		case OPR_LEQ:
			break;
		case OPR_GEQ:
			break;
		case OPR_GT:
			break;
		case OPR_LT:
			break;
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

map<string,predicate *> query_predicate::predicates;

query_predicate::query_predicate(string nm, list<query_val *> p) {
	name = nm;
	params = p;
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
	predicate *p = predicates[name];

	if(!p) {
		cerr << "query_predicate()::check: unknown predicate '" << name << "'" << endl;
		exit(1);
	}

	return p->invoke(n,params);
}

void query_predicate::register_predicate(string name, predicate *p) {
	predicates[name] = p;
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
	rerror("query_scope::check") << "NOT IMPLEMENTED" << endl;
	exit(1);
	return false;
}




