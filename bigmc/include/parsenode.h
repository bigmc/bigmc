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

class parsenode {
	int filepos;
public:
	int type;
	parsenode();
	~parsenode();
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
	controlnode *prefix;
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

class reactionnode : public parsenode {
public:
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



#endif
