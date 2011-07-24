#ifndef _PREDICATE_H
#define _PREDICATE_H

class query_val;

typedef bool (*predfn_check_t)(node*,list<query_val*>);
typedef int (*predfn_eval_t)(node*,list<query_val*>);

class predicate {
	predfn_check_t pred_check;
	predfn_eval_t pred_eval;
	void *handle;
public:
	predicate(string name, char *filename);
	~predicate();
	bool invoke_check(node *n, list<query_val *> params);
	int invoke_eval(node *n, list<query_val *> params);
};

// Make it a bit prettier to define predicates in modules
#define PRED_EXPORT extern "C"

#define PRED_FAIL_NO_EVAL(x) do { rerror("predicate") << "Type error: pred_" << x << \
				" is of type 'bool', but is being used as type 'int'" << endl; \
				exit(1); \
				} while(0);


#define PRED_FAIL_NO_CHECK(x) do { rerror("predicate") << "Type error: pred_" << x << \
				" is of type 'int', but is being used as type 'bool'" << endl; \
				exit(1); \
				} while(0);

#define PRED_FAIL_PARAM(x,y,z)  do { rerror("predicate") << "Type error: pred_" << x << \
				"  is of type '" << y << "' but was applied as '" << \
				z << "'" << endl; \
				exit(1); \
				} while(0);


#endif
