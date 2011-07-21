#ifndef _QUERY_H
#define _QUERY_H

class query {
public:
	query();
	~query();
	virtual string to_string();
	virtual bool check(node *n);
};

class query_and : public query {
public:
	query *lhs;
	query *rhs;
	query_and(query *l, query *r);
	~query_and();
	string to_string();
	bool check(node *n);
};

class query_or : public query {
public:
	query *lhs;
	query *rhs;
	query_or(query *l, query *r);
	~query_or();
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
public:
	term *param;
	string name;
	query_predicate(string nm, term *l);
	~query_predicate();
	string to_string();
	bool check(node *n);
};

#endif
