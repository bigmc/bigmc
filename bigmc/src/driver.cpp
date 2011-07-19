using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <map>

#include <bigmc.h>

void driver::check(bigraph *b) {
	cerr << "BUG: driver::check()" <<  endl;
	if(!b || !b->get_root(0)) {
		cout << "driver::check(): NULL" << endl;
		return;
	}

	cout << "driver::check():\n" << b->to_string() << endl;

	set<match *> matches = b->find_matches();
	cout << "driver::check(): found " << matches.size() << " matches" << endl;

	/*
	cout << " * Original:" << endl;
	cout << b->get_root(0)->to_string() << endl;
	
	for(set<match *>::iterator it = matches.begin(); it != matches.end(); ++it) {
		bigraph *b2 = b->apply_match(*it);
		cout << " * Applied match:" << endl;
		cout << b2->get_root(0)->to_string() << endl;
	}*/

}
