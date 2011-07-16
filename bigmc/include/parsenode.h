#ifndef _PARSENODE_H
#define _PARSENODE_H

class parsenode {
	int filepos;
public:
	parsenode();
	~parsenode();
	virtual string to_string();
	virtual bool is_valid();
	virtual vector<parsenode *> get_children();
};

class prefixnode : public parsenode {
	parsenode *prefix;
	parsenode *suffix;
public:
	prefixnode(parsenode *p, parsenode *q);
	~prefixnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class parallelnode : public parsenode {
	parsenode *lhs;
	parsenode *rhs;
public:
	parallelnode(parsenode *l, parsenode *r);
	~parallelnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class reactionnode : public parsenode {
	parsenode *redex;
	parsenode *reactum;
public:
	reactionnode(parsenode *red, parsenode *reac);
	~reactionnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class namenode : public parsenode {
	char *nme;
	name hash;
public:
	namenode(char *id);
	~namenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class holenode : public parsenode {
	int n;
public:
	holenode(int id);
	~holenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class seqnode : public parsenode {
	parsenode *lhs;
	parsenode *rhs;
public:
	seqnode(parsenode *l, parsenode *r);
	~seqnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class interfacenode : public parsenode {
	parsenode *name;
	bool outer;
public:
	interfacenode(parsenode *n, bool is_outer);
	~interfacenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class signaturenode : public parsenode {
	parsenode *name;
	bool active;
	int arity;
public:
	signaturenode(parsenode *n, bool is_active, int ar);
	~signaturenode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

class controlnode : public parsenode {
	parsenode *name;
	parsenode *links;
public:
	controlnode(parsenode *id, parsenode *linkseq);
	~controlnode();
	string to_string();
	bool is_valid();
	vector<parsenode *> get_children();
};

#endif
