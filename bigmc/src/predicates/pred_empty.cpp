using namespace std;
#include <bigmc.h>
#include <bigraph.h>
#include <node.h>
#include <iostream>
#include <list>

PRED_EXPORT bool pred_check_empty(node *n, list<query_val *> param) {
	std::cout << "pred_empty(): called" << std::endl;

	return (n->bg->get_root(0)->size() == 0);
}

PRED_EXPORT int pred_eval_empty(node *n, list<query_val *> param) {
	std::cout << "pred_eval_empty(): called" << std::endl;

	PRED_FAIL_NO_EVAL("empty");

	return 0;
}
