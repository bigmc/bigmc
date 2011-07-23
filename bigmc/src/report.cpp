using namespace std;
#include <string>
#include <set>
#include <map>
#include <iostream>

#include <bigmc.h>

std::ostream &report(int level, char *file, char *lineno, string module) {
	if(level == REPORT_INFO) {
		cout << "[" << module << "] ";
		return cout;
	}

	cerr << "[" << module << "] ";
	return cerr;
}

std::ostream &rerror(string module) {
	return report(REPORT_FATAL, NULL, 0, module);
}

std::ostream &rwarning(string module) {
	return report(REPORT_WARN, NULL, 0, module);
}

std::ostream &rinfo(string module) {
	return report(REPORT_INFO, NULL, 0, module);
}

