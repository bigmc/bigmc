#ifndef _QUERY_H
#define _QUERY_H

class query {
public:
	query();
	~query();
	virtual string to_string();
	virtual bool check(node *n);
};

class query_val : public query {
public:
	query_val();
	~query_val();
	virtual string to_string();
	virtual bool check(node *n);
};

class query_num : public query_val {
public:
	query_num(int d);
	~query_num();
	int value;
	string to_string();
	bool check(node *n);
};

class query_term : public query_val {
public:
	query_term(term *t);
	~query_term();
	term *data;
	string to_string();
	bool check(node *n);
};

class query_id : public query_val {
public:
	query_id(string id);
	~query_id();
	string name;
	string to_string();
	bool check(node *n);
};

class query_bin : public query {
public:
	query *lhs;
	query *rhs;
	int oper;
	query_bin(query *l, int opr, query *r);
	~query_bin();
	string to_string();
	bool check(node *n);
};

class query_not : public query {
public:
	query *lhs;
	query_not(query *l);
	~query_not();
	string to_string();
	bool check(node *n);
};


class query_predicate : public query {
	static map<string,predicate *> predicates;
public:
	list<query_val *> params;
	string name;
	query_predicate(string nm, list<query_val*> l);
	~query_predicate();
	string to_string();
	bool check(node *n);
	static void register_predicate(string name, predicate *p);
};

class query_scope : public query {
public:
	query_scope(string nm, query *q);
	~query_scope();
	string name;
	query *qry;
	string to_string();
	bool check(node *n);
};



#endif
