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
#ifndef _PARSENODE_H
#define _PARSENODE_H

#define NODE_PARSE 0
#define NODE_PREFIX 1
#define NODE_PARALLEL 2
#define NODE_REACTION 4
#define NODE_NAME 8
#define NODE_HOLE 16
#define NODE_SEQ 32
#define NODE_INTERFACE 64
#define NODE_SIGNATURE 128
#define NODE_CONTROL 256
#define NODE_PROPERTY 512
#define NODE_BIN	1024
#define NODE_QUERY	2048
#define NODE_NOT	4096
#define NODE_PRED	8192
#define NODE_NUM	16384
#define NODE_IF		32768
#define NODE_REGION	65536
#define NODE_NIL	131072

#define OPR_NEQ	1
#define OPR_EQ	2
#define OPR_LEQ	3
#define OPR_GEQ	4
#define OPR_GT	5
#define OPR_LT	6
#define OPR_AND	7
#define OPR_OR	8

class parsenode {
	int filepos;
public:
	int type;
	parsenode();
	virtual ~parsenode();
	virtual string to_string();
	virtual bool is_valid();
	virtual vector<parsenode *> get_children();
};

class namenode : public parsenode {
public:
	char *nme;
	name hash;
	namenode(char *id);
	~namenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class controlnode : public parsenode {
public:
	parsenode *name;
	parsenode *links;
	controlnode(parsenode *id, parsenode *linkseq);
	controlnode(namenode *id, parsenode *linkseq);
	~controlnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class prefixnode : public parsenode {
public:
	parsenode *prefix;
	parsenode *suffix;
	prefixnode(parsenode *p, parsenode *q);
	prefixnode(controlnode *p, parsenode *q);
	~prefixnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class parallelnode : public parsenode {
public:
	parsenode *lhs;
	parsenode *rhs;
	parallelnode(parsenode *l, parsenode *r);
	~parallelnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class regionnode : public parsenode {
public:
	parsenode *lhs;
	parsenode *rhs;
	regionnode(parsenode *l, parsenode *r);
	~regionnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class reactionnode : public parsenode {
public:
	string name;
	parsenode *redex;
	parsenode *reactum;
	reactionnode(parsenode *red, parsenode *reac);
	~reactionnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};



class holenode : public parsenode {
public:
	int n;
	holenode(int id);
	~holenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class nilnode : public parsenode {
public:
	nilnode();
	~nilnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class seqnode : public parsenode {
public:
	parsenode *lhs;
	parsenode *rhs;
	seqnode(parsenode *l, parsenode *r);
	~seqnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class interfacenode : public parsenode {
public:
	parsenode *name;
	bool outer;
	interfacenode(parsenode *n, bool is_outer);
	~interfacenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class signaturenode : public parsenode {
public:
	parsenode *name;
	bool active;
	int arity;
	signaturenode(parsenode *n, bool is_active, int ar);
	~signaturenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class propertynode : public parsenode {
public:
	parsenode *prop;
	string name;
	propertynode(char *nm, parsenode *p);
	~propertynode();
	string to_string();
};


class binnode : public parsenode {
public:
	parsenode *lprop;
	parsenode *rprop;
	int oper;
	binnode(parsenode *l, int opr, parsenode *r);
	~binnode();
	string to_string();
};

class notnode : public parsenode {
public:
	parsenode *prop;
	notnode(parsenode *p);
	~notnode();
	string to_string();
};

class prednode : public parsenode {
public:
	parsenode *prop;
	string name;
	prednode(char *name, parsenode *n);
	~prednode();
	string to_string();
};

class numnode : public parsenode {
public:
	int data;
	numnode(int d);
	~numnode();
	string to_string();
};

class querynode : public parsenode {
public:
	parsenode *prop;
	string name;
	querynode(char *name, parsenode *n);
	~querynode();
	string to_string();
};

class ifnode : public parsenode {
public:
	parsenode *cond;
	parsenode *tbranch;
	parsenode *fbranch;
	ifnode(parsenode *c, parsenode *t, parsenode *f);
	~ifnode();
	string to_string();
};


#endif
