#include <bigmc.h>
#include <iostream>

PRED_EXPORT bool pred_equal(node *n, term *param) {
	std::cout << "pred_equal(): called" << std::endl;

	return false;
}
