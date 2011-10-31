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
using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>
#include <assert.h>

#include <config.h>
#include <globals.h>
#include <report.h>
#include <match.h>
#include <term.h>
#include <reactionrule.h>
#include <bigraph.h>
#include <matcher.h>
#include <calculation.h>



term *calculation::calculate(prefix *t, int op) {
	if(t->get_control() == bigraph::control_from_string("+")) {
		return calculate(t->get_suffix(), OP_SUM);
	}

	return t;
}

term *calculation::calculate(parallel *t, int op) {
	set<term*> terms = t->get_children();
	int sum = 0;

	for(set<term*>::iterator i = terms.begin(); i  != terms.end(); i++) {
		term *nt = calculate(*i,0);
		if(nt->type == TNUM)
			//sum += nt->value;
			sum = sum;
		else {
			cout << "Error: invalid calculational BRS term: " << t->to_string() << endl;
			exit(1);
		}
	}

	return new num(sum);
}


term *calculation::calculate(term *t, int op) {
	switch(t->type) {
		case TPREF:
			return calculate((prefix*)t, op);
		case TPAR:
			return calculate((parallel*)t, op);
		default:
			return t;
	}
}

bigraph *calculation::apply(bigraph *b) {
	return b;
}


