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

#ifndef _WIN32

#include <dlfcn.h>

#else

#include <windows.h>

#endif


predicate::predicate(string name, char *filename) {
	pred_check = NULL;
	pred_eval = NULL;

	if(filename[0] == '/') {
		if(global_cfg.verbose_level) 
			rinfo("predicate::predicate") << "loading predicate from " <<
				filename << endl;

		#ifndef _WIN32
		handle = dlopen(filename, RTLD_LAZY);
		#else
		handle = LoadLibrary(filename);
		#endif
	} else {
		char buf[2048];
		sprintf(buf,"%s/lib/%s", global_cfg.bigmc_home, filename);
		if(global_cfg.verbose_level) 
		rinfo("predicate::predicate") << "loading predicate from " << buf << endl;
		#ifndef _WIN32
		handle = dlopen(buf, RTLD_LAZY);
		#else
		handle = LoadLibrary(buf);
		#endif
	}

	#ifndef _WIN32
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
	#else
	// WINDOWS

	if (!handle) {
	        rerror("predicate::predicate") << "DLL loading failed" << endl;
		exit(1);
	}

	pred_check = (predfn_check_t) GetProcAddress(handle, ("pred_check_" + name).c_str());
	pred_eval = (predfn_eval_t) GetProcAddress(handle, ("pred_eval_" + name).c_str());

	#endif

}

predicate::~predicate() {
	if(handle)
		#ifndef _WIN32
		dlclose(handle);
		#else
		FreeLibrary(handle);
		#endif
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


