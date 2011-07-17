using namespace std;
#include <string>
#include <sstream>
#include <set>

#include <bigmc.h>

hole::hole(int id) {
	index = id;
}

hole::~hole() {

}

void hole::add(node *n) {
	fprintf(stderr,"BUG: Adding things to a hole makes no sense!\n");
	exit(1);
}

string hole::to_string() {
	std::string s;
	std::stringstream out;
	out << index;
	return "$" + out.str();
}
