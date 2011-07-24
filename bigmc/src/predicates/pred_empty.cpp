using namespace std;
#include <bigmc.h>
#include <bigraph.h>
#include <node.h>
#include <iostream>
#include <list>

PRED_EXPORT int pred_empty(node *n, list<query_val *> param) {
	std::cout << "pred_empty(): called" << std::endl;

	return (n->bg->get_root(0)->size() == 0);
}
