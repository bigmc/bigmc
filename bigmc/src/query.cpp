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

// AND

query_and::query_and(query *l, query *r) {
	lhs = l;
	rhs = r;
}

query_and::~query_and() {

}

string query_and::to_string() {
	return lhs->to_string() + " && " + rhs->to_string();
}

bool query_and::check(node *n) {
	// TODO: Emit diagnostic information here in event of failure
	return lhs->check(n) && rhs->check(n);
}

// OR

query_or::query_or(query *l, query *r) {
	lhs = l;
	rhs = r;
}

query_or::~query_or() {

}

string query_or::to_string() {
	return lhs->to_string() + " || " + rhs->to_string();
}

bool query_or::check(node *n) {
	// TODO: Emit diagnostic information here in event of failure
	return lhs->check(n) || rhs->check(n);
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

query_predicate::query_predicate(string nm, term *l) {
	name = nm;
	param = l;
}

query_predicate::~query_predicate() {

}

string query_predicate::to_string() {
	string s = name;
	if(param == NULL)
		return s + "()";
	else
		return s + "(" + param->to_string() + ")";
}

bool query_predicate::check(node *n) {
	if(name == "empty") {
		return (n->bg->get_root(0)->size() == 0);
	}

	if(g_debug) cout << "query_predicate::check(): NOT IMPLEMENTED" << endl;
	return false;
}


