using namespace std;
#include <string>
#include <set>
#include <map>

#include <bigmc.h>

reactionrule::reactionrule(term *red, term *reac) {
	redex = red;
	reactum = reac;
}

reactionrule::~reactionrule() {

}

string reactionrule::to_string() {
	return redex->to_string() + " -> " + reactum->to_string();
}

