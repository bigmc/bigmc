using namespace std;
#include <string>
#include <set>
#include <map>

#include <bigmc.h>

reactionrule::reactionrule(node *red, node *reac) {
	redex = red;
	reactum = reac;
}

reactionrule::~reactionrule() {

}

