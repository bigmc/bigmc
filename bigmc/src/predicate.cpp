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


