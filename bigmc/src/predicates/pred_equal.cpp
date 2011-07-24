#include <bigmc.h>
#include <iostream>
#include <list>

PRED_EXPORT int pred_equal(node *n, list<query_val *> param) {
	std::cout << "pred_equal(): called" << std::endl;

	return false;
}
