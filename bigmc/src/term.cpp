using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <bigmc.h>

term::term() {
	type = 0;
}

term::~term() {
}

string term::to_string() {
	return "<untyped term>";
}

set<match *> term::find_matches(reactionrule *r) {
	assert(0);
	switch(type) {
		case TPREF: {
			cout << "term::find_matches(): prefix\n";
			break;
		} 
		case TPAR:
			cout << "term::find_matches(): par\n";
		break;
		case THOLE:
			cout << "term::find_matches(): hole\n";

		break;
		case TNIL:
			cout << "term::find_matches(): nil\n";
		break;
		default:
			cerr << "term::find_matches(): Unknown node type " << type << endl;
			exit(1);
		break;
	}

	return set<match*>();
}

parallel::parallel(set<term *>l) {
	terms = l;
	type = TPAR;
}

parallel::~parallel() {
}

string parallel::to_string() {
	string s = "";

	int k = 0;
	for(set<term *>::iterator i = terms.begin(); i!=terms.end(); ++i) {
		k++;

		s += (*i)->to_string();

		if(k < terms.size())
			s += " | ";
	}

	return s;
}

set<match *> parallel::find_matches(reactionrule *r) {
	cout << "parallel::find_matches()" << endl;
	return set<match *>();
}

set<term *> parallel::get_children() {
	return terms;
}


prefix::prefix(control c, vector<name> ports, term *suff) {
	ctrl = c;
	arity = bigraph::arity(c);
	active = bigraph::activity(c);

	port = ports;
	suffix = suff;

	type = TPREF;

	if(port.size() > arity) {
		cerr << "Error: control " << bigraph::control_to_string(c) << " has arity " << arity << " but " << port.size() << " ports have been linked!" << endl;
		exit(1);
	}
}

prefix::~prefix() {
}

string prefix::to_string() {
	string nm = "";

	if(port.size() == 0)
		nm = "";
	else {
		for(int i = 0; i<port.size(); i++) {
			if(port[i] == 0) {
				nm += "_";
			} else {
				nm += bigraph::name_to_string(port[i]);
			}
			if(i != port.size()-1)
				nm += ",";
		}

		if(nm != "") nm = "[" + nm + "]";
	}
	if(suffix != NULL)
		return bigraph::control_to_string(ctrl) + nm + "." + suffix->to_string();
	else
		return bigraph::control_to_string(ctrl) + nm + ".NULL";
}

term *prefix::get_suffix() {
	return suffix;
}

hole::hole(int idx) {
	index = idx;
	type = THOLE;
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
	type = TNIL;
}

nil::~nil() {
}

string nil::to_string() {
	return "nil";
}


