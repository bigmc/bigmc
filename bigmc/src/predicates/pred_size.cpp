using namespace std;
#include <bigmc.h>
#include <bigraph.h>
#include <node.h>
#include <iostream>
#include <list>

PRED_EXPORT bool pred_check_size(node *n, list<query_val *> param) {
	PRED_FAIL_NO_CHECK("empty");
	return false;
}

PRED_EXPORT int pred_eval_size(node *n, list<query_val *> param) {
	return n->bg->get_root(0)->size();
}
