#include <bigmc.h>
#include <iostream>
#include <list>

PRED_EXPORT bool pred_check_equal(node *n, list<query_val *> param) {
	std::cout << "pred_equal(): called" << std::endl;

	return false;
}

PRED_EXPORT int pred_eval_equal(node *n, list<query_val *> param) {
	std::cout << "pred_eval_equal(): called" << std::endl;

	PRED_FAIL_NO_EVAL("eval");

	return 0;
}
