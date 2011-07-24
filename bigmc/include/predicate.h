#ifndef _PREDICATE_H
#define _PREDICATE_H

class query_val;

typedef int (*predfn_t)(node*,list<query_val*>);

class predicate {
	predfn_t pred_impl;
	void *handle;
public:
	predicate(string name, char *filename);
	~predicate();
	bool invoke(node *n, list<query_val *> params);
};

// Make it a bit prettier to define predicates in modules
#define PRED_EXPORT extern "C"

#endif
