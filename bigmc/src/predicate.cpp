using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <bigmc.h>
#include <dlfcn.h>

predicate::predicate(string name, char *filename) {
	cout << "predicate::predicate(): loading predicate module from " << filename << endl;
	pred_impl = NULL;

	handle = dlopen(filename, RTLD_LAZY);
	if (!handle) {
	        cerr << "predicate::predicate(): loading failed: " << dlerror() << '\n';
		exit(1);
	}

	dlerror();
	pred_impl = (predfn_t) dlsym(handle, ("pred_" + name).c_str());
	const char *dlsym_error = dlerror();

	if (dlsym_error) {
		cerr << "predicate::predicate(): error loading symbol 'pred_" << name << 
			"': " << dlsym_error << '\n';
		dlclose(handle);
		exit(1);
	}
}

predicate::~predicate() {
	if(handle)
		dlclose(handle);
}

bool predicate::invoke(node *n,term *param) {
	if(!pred_impl) {
		cout << "predicate::invoke(): Implementation is NULL!" << endl;
		exit(1);
	}

	return pred_impl(n,param);
}

