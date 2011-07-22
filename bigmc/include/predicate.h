#ifndef _PREDICATE_H
#define _PREDICATE_H

typedef bool (*predfn_t)(node*,term*);

class predicate {
	predfn_t pred_impl;
	void *handle;
public:
	predicate(string name, char *filename);
	~predicate();
	bool invoke(node *n, term *param);
};

// Make it a bit prettier to define predicates in modules
#define PRED_EXPORT extern "C"

#endif
