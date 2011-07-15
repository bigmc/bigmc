using namespace std;
#include <string>
#include <set>

#include <bigmc.h>

bigraph::bigraph(int roots) {
	root = new node[roots];
	nregions = roots;
}

bigraph::~bigraph() {
	// should maybe recursively delete nodes
	delete [] root;
}
