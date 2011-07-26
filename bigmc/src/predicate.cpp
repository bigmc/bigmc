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
#include <sstream>
#include <assert.h>
#include <bigmc.h>
#include <dlfcn.h>

predicate::predicate(string name, char *filename) {
	pred_check = NULL;
	pred_eval = NULL;

	if(filename[0] == '/') {
		rinfo("predicate::predicate") << "loading predicate from " << filename << endl;
		handle = dlopen(filename, RTLD_LAZY);
	} else {
		char buf[2048];
		sprintf(buf,"%s/lib/%s", global_cfg.bigmc_home, filename);
		rinfo("predicate::predicate") << "loading predicate from " << buf << endl;
		handle = dlopen(buf, RTLD_LAZY);
	}

	if (!handle) {
	        rerror("predicate::predicate") << "loading failed: " << dlerror() << endl;
		exit(1);
	}

	dlerror();
	pred_check = (predfn_check_t) dlsym(handle, ("pred_check_" + name).c_str());
	const char *dlsym_error = dlerror();

	if (dlsym_error) {
		cerr << "predicate::predicate(): error loading symbol 'pred_check_" << name << 
			"': " << dlsym_error << '\n';
		dlclose(handle);
		exit(1);
	}

	dlerror();
	pred_eval = (predfn_eval_t) dlsym(handle, ("pred_eval_" + name).c_str());
	const char *dlsym_error2 = dlerror();

	if (dlsym_error2) {
		cerr << "predicate::predicate(): error loading symbol 'pred_eval_" << name << 
			"': " << dlsym_error2 << '\n';
		dlclose(handle);
		exit(1);
	}

}

predicate::~predicate() {
	if(handle)
		dlclose(handle);
}

bool predicate::invoke_check(node *n,list<query_val *> param) {
	if(!pred_check) {
		cout << "predicate::invoke_check(): Implementation is NULL!" << endl;
		exit(1);
	}

	return pred_check(n,param);
}


int predicate::invoke_eval(node *n,list<query_val *> param) {
	if(!pred_eval) {
		cout << "predicate::invoke_check(): Implementation is NULL!" << endl;
		exit(1);
	}

	return pred_eval(n,param);
}


