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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <globals.h>
#include <parsenode.h>

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
	if(p->type == NODE_NUM)
		prefix = (numnode *)p;
	else
		prefix = dynamic_cast<controlnode *>(p);
	assert(prefix != NULL);
	suffix = q;
	type = NODE_PREFIX;
}

prefixnode::~prefixnode() {
	delete prefix;
	if(suffix)
		delete suffix;
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
	delete lhs;
	delete rhs;
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

// REGION

regionnode::regionnode(parsenode *l, parsenode *r) {
	lhs = l;
	rhs = r;
	type = NODE_REGION;
}

regionnode::~regionnode() {
	delete lhs;
	delete rhs;
}

string regionnode::to_string() {
	return lhs->to_string() + " || " + rhs->to_string();
}

bool regionnode::is_valid() {
	return lhs && rhs && lhs->is_valid() && rhs->is_valid();
}

vector<parsenode *> regionnode::get_children() {
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
	name = "";
}

reactionnode::~reactionnode() {
	delete redex;
	delete reactum;
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
	nme = id;
	// TODO compute hash
	type = NODE_NAME;
}

namenode::~namenode() {
	free(nme);
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

// NIL

nilnode::nilnode() {
	type = NODE_NIL;
}

nilnode::~nilnode() {
}

string nilnode::to_string() {
	return "nil";
}

bool nilnode::is_valid() {
	return true;
}

vector<parsenode *> nilnode::get_children() {
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
	delete lhs;
	delete rhs;
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
	delete name;
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
	delete name;
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
	delete name;
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
	delete prop;
}

string propertynode::to_string() {
	return "%property " + name + " " + prop->to_string();
}

// BINARY OP 

binnode::binnode(parsenode *l, int opr, parsenode *r) {
	lprop = l;
	rprop = r;
	oper = opr;
	type = NODE_BIN;
}

binnode::~binnode() {
	delete lprop;
	delete rprop;
}

string binnode::to_string() {
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

	return lprop->to_string() + " " + op + " " + rprop->to_string();
}


// NOT 

notnode::notnode(parsenode *l) {
	prop = l;
	type = NODE_NOT;
}

notnode::~notnode() {
	delete prop;
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
	delete prop;
}

string prednode::to_string() {
	return name + "(" + ((prop != NULL) ? prop->to_string() : "") + ")";
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

// QUERY

querynode::querynode(char *n, parsenode *l) {
	prop = l;
	name = string(n);
	type = NODE_QUERY;
}

querynode::~querynode() {
	delete prop;
}

string querynode::to_string() {
	return "$" + name + "->" + prop->to_string();
}

// IF

ifnode::ifnode(parsenode *c, parsenode *t, parsenode *f) {
	cond = c;
	tbranch = t;
	fbranch = f;
	type = NODE_IF;
}

ifnode::~ifnode() {
	delete cond;
	delete tbranch;
	delete fbranch;
}

string ifnode::to_string() {
	return "(if " + cond->to_string() + " then " + tbranch->to_string() +
		" else " + fbranch->to_string() + ")";
}






