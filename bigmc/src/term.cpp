using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <sstream>

#include <bigmc.h>

term::term() {
}

term::~term() {
}

string term::to_string() {
	return "<untyped term>";
}

parallel::parallel(set<term *>l) {
	terms = l;
}

parallel::~parallel() {
}

string parallel::to_string() {
	//return "(" + lhs->to_string() + " | " + rhs->to_string() + ")";
	return "FIXME: broken parallel to_string()";
}

prefix::prefix(control c, vector<name> ports, term *suff) {
	ctrl = c;
	arity = bigraph::arity(c);
	active = bigraph::activity(c);

	port = ports;
	suffix = suff;
}

prefix::~prefix() {
}

string prefix::to_string() {
	return bigraph::control_to_string(ctrl) + "." + suffix->to_string();
}

hole::hole(int idx) {
	index = idx;
}

hole::~hole() {
}

string hole::to_string() {
	std::string s;
	std::stringstream out;
	out << index;
	return "$" + out.str();
}

nil::nil() {
}

nil::~nil() {
}

string nil::to_string() {
	return "nil";
}


