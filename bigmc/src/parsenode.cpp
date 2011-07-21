using namespace std;
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <bigmc.h>

parsenode::parsenode() {
	filepos = 0;
	type = NODE_PARSE;
}

parsenode::~parsenode() {
}

string parsenode::to_string() {
	return "<parsenode>";
}

bool parsenode::is_valid() {
	return false;
}

vector<parsenode *> parsenode::get_children() {
	vector<parsenode *> v;
	return v;
}

// PREFIX

prefixnode::prefixnode(controlnode *p, parsenode *q) {
	prefix = p;
	suffix = q;
	type = NODE_PREFIX;
}

prefixnode::prefixnode(parsenode *p, parsenode *q) {
	prefix = dynamic_cast<controlnode *>(p);
	assert(prefix != NULL);
	suffix = q;
	type = NODE_PREFIX;
}

prefixnode::~prefixnode() {
}

string prefixnode::to_string() {
	if(suffix)
		return prefix->to_string() + "." + suffix->to_string();
	else 
		return prefix->to_string() + ".nil";
}

bool prefixnode::is_valid() {
	return prefix && suffix && prefix->is_valid() && suffix->is_valid();
}

vector<parsenode *> prefixnode::get_children() {
	vector<parsenode *> v;
	v.push_back(prefix);
	v.push_back(suffix);
	return v;
}

// PARALLEL

parallelnode::parallelnode(parsenode *l, parsenode *r) {
	lhs = l;
	rhs = r;
	type = NODE_PARALLEL;
}

parallelnode::~parallelnode() {
}

string parallelnode::to_string() {
	return lhs->to_string() + " | " + rhs->to_string();
}

bool parallelnode::is_valid() {
	return lhs && rhs && lhs->is_valid() && rhs->is_valid();
}

vector<parsenode *> parallelnode::get_children() {
	vector<parsenode *> v;
	v.push_back(lhs);
	v.push_back(rhs);
	return v;
}

// REACTION

reactionnode::reactionnode(parsenode *red, parsenode *reac) {
	redex = red;
	reactum = reac;
	type = NODE_REACTION;
}

reactionnode::~reactionnode() {
}

string reactionnode::to_string() {
	return redex->to_string() + " -> " + reactum->to_string();
}

bool reactionnode::is_valid() {
	return redex && reactum && redex->is_valid() && reactum->is_valid();
}

vector<parsenode *> reactionnode::get_children() {
	vector<parsenode *> v;
	v.push_back(redex);
	v.push_back(reactum);
	return v;
}


// NAME 

namenode::namenode(char *id) {
	nme = strdup(id);
	// TODO compute hash
	type = NODE_NAME;
}

namenode::~namenode() {
}

string namenode::to_string() {
	return string(nme);
}

bool namenode::is_valid() {
	return nme != NULL;
}

vector<parsenode *> namenode::get_children() {
	vector<parsenode *> v;
	return v;
}

// HOLE 

holenode::holenode(int id) {
	n = id;
	type = NODE_HOLE;
}

holenode::~holenode() {
}

string holenode::to_string() {
	std::string s;
	std::stringstream out;
	out << n;
	return "$" + out.str();
}

bool holenode::is_valid() {
	return true;
}

vector<parsenode *> holenode::get_children() {
	vector<parsenode *> v;
	return v;
}


// SEQ

seqnode::seqnode(parsenode *l, parsenode *r) {
	lhs = l;
	rhs = r;
	type = NODE_SEQ;
}

seqnode::~seqnode() {
}

string seqnode::to_string() {
	return lhs->to_string() + ", " + rhs->to_string();
}

bool seqnode::is_valid() {
	return lhs && rhs && lhs->is_valid() && rhs->is_valid();
}

vector<parsenode *> seqnode::get_children() {
	vector<parsenode *> v;
	v.push_back(lhs);
	v.push_back(rhs);
	return v;
}

// INTERFACE

interfacenode::interfacenode(parsenode *n, bool is_outer) {
	name = n;
	outer = is_outer;
	type = NODE_INTERFACE;
}

interfacenode::~interfacenode() {
}

string interfacenode::to_string() {
	if(outer) {
		return "%outer " + name->to_string();
	} else
		return "%inner " + name->to_string();
}

bool interfacenode::is_valid() {
	return name && name->is_valid();
}

vector<parsenode *> interfacenode::get_children() {
	vector<parsenode *> v;
	v.push_back(name);
	return v;
}

// SIGNATURE

signaturenode::signaturenode(parsenode *n, bool is_active, int ar) {
	name = n;
	active = is_active;
	arity = ar;
	type = NODE_SIGNATURE;
}

signaturenode::~signaturenode() {
}

string signaturenode::to_string() {
	std::string s;
	std::stringstream out;
	out << arity;
	s = out.str();

	if(active) {
		return "%active " + name->to_string() + " : " + s;
	} else
		return "%passive " + name->to_string() + " : " + s;
}

bool signaturenode::is_valid() {
	return name && name->is_valid();
}

vector<parsenode *> signaturenode::get_children() {
	vector<parsenode *> v;
	v.push_back(name);
	return v;
}

// CONTROL


controlnode::controlnode(parsenode *id, parsenode *linkseq) {
	name = id;
	links = linkseq;
	type = NODE_CONTROL;
}

controlnode::controlnode(namenode *id, parsenode *linkseq) {
	name = id;
	links = linkseq;
	type = NODE_CONTROL;
}

controlnode::~controlnode() {
}

string controlnode::to_string() {
	if(links != NULL) 
		return name->to_string() + "(" + links->to_string() + ")";
	else
		return name->to_string();
}

bool controlnode::is_valid() {
	return name && name->is_valid();
}

vector<parsenode *> controlnode::get_children() {
	vector<parsenode *> v;
	v.push_back(name);
	if(links != NULL)
		v.push_back(links);
	return v;
}

// PROPERTY

propertynode::propertynode(char *nm, parsenode *p) {
	name = string(nm);
	prop = p;
	type = NODE_PROPERTY;
}

propertynode::~propertynode() {
	
}

string propertynode::to_string() {
	return "%property " + name + " " + prop->to_string();
}

// AND

andnode::andnode(parsenode *l, parsenode *r) {
	lprop = l;
	rprop = r;
	type = NODE_AND;
}

andnode::~andnode() {
	
}

string andnode::to_string() {
	return lprop->to_string() + " && " + rprop->to_string();
}


// OR 

ornode::ornode(parsenode *l, parsenode *r) {
	lprop = l;
	rprop = r;
	type = NODE_OR;
}

ornode::~ornode() {
	
}

string ornode::to_string() {
	return lprop->to_string() + " || " + rprop->to_string();
}

// NOT 

notnode::notnode(parsenode *l) {
	prop = l;
	type = NODE_NOT;
}

notnode::~notnode() {
	
}

string notnode::to_string() {
	return "!" + prop->to_string();
}

// PRED 

prednode::prednode(char *n, parsenode *l) {
	prop = l;
	name = string(n);
	type = NODE_PRED;
}

prednode::~prednode() {
	
}

string prednode::to_string() {
	return name + "(" + prop->to_string() + ")";
}

// NUM 

numnode::numnode(int d) {
	data = d;
	type = NODE_NUM;
}

numnode::~numnode() {
	
}

string numnode::to_string() {
	stringstream out;
	out << data;
	return out.str();
}







