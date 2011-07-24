using namespace std;
#include <bigmc.h>
#include <bigraph.h>
#include <node.h>
#include <iostream>
#include <list>

PRED_EXPORT bool pred_check_matches(node *n, list<query_val *> param) {
	if(param.size() == 0 || param.size() > 1) {
		PRED_FAIL_PARAM("matches", "term -> bool", "_ -> bool");
		exit(1);
	}

	query_term *t = dynamic_cast<query_term *>(param.front());

	if(t == NULL) {
		PRED_FAIL_PARAM("matches", "term -> bool", "??? -> bool");
		exit(1);
	}

	return term::matches(n->bg->get_root(0), t->data);
}

PRED_EXPORT int pred_eval_matches(node *n, list<query_val *> param) {
	PRED_FAIL_NO_EVAL("matches");
	return 0;
}
