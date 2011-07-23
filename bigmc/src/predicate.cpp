using namespace std;
#include <string>
#include <set>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <bigmc.h>
#include <dlfcn.h>

predicate::predicate(string name, char *filename) {
	pred_impl = NULL;

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

