using namespace std;
#include <bigmc.h>
#include <bigraph.h>
#include <node.h>
#include <iostream>

PRED_EXPORT bool pred_empty(node *n, term *param) {
	std::cout << "pred_empty(): called" << std::endl;

	return (n->bg->get_root(0)->size() == 0);
}
