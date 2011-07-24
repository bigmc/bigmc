#include <bigmc.h>
#include <iostream>
#include <list>

PRED_EXPORT int pred_terminal_contains(node *n, list<query_val *> param) {
	std::cout << "pred_terminal_contains(): called on " << n->bg->get_root(0)->to_string();


	if(!n->terminal) {
		std::cout << ": not terminal: " << n->hash << std::endl; 
		return true;
	}


	std::cout << ": is terminal!" << n->hash << std::endl; 

	return false;
}
