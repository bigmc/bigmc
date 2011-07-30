/*******************************************************************************
*
* Copyright (C) 2011 Gian Perrone (http://itu.dk/~gdpe)
* 
* BigMC - A bigraphical model checker (http://bigraph.org/bigmc).
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
* USA.
*********************************************************************************/
#include <bigmc.h>
#include <iostream>
#include <list>

PRED_EXPORT bool pred_check_terminal_contains(node *n, list<query_val *> param) {
	std::cout << "pred_terminal_contains(): called on " << n->bg->get_root(0)->to_string();


	if(!n->terminal) {
		std::cout << ": not terminal: " << n->hash << std::endl; 
		return true;
	}


	std::cout << ": is terminal!" << n->hash << std::endl; 

	return false;
}

PRED_EXPORT int pred_eval_terminal_contains(node *n, list<query_val *> param) {
	PRED_FAIL_NO_EVAL("terminal_contains");
	return 0;
}

