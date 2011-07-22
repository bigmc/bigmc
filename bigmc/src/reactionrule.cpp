using namespace std;
#include <string>
#include <set>
#include <map>
#include <iostream>

#include <bigmc.h>

reactionrule::reactionrule(term *red, term *reac) {
	redex = red;
	reactum = reac;
}

reactionrule::~reactionrule() {

}

string reactionrule::to_string() {
	cout << "reactionrule::to_string(): " << endl;
	cout << redex << endl;
	cout << reactum << endl;
	if(redex != NULL && reactum != NULL)
		return redex->to_string() + " -> " + reactum->to_string();
	if(redex != NULL)
		return redex->to_string() + " -> NULL";
	if(reactum != NULL)
		return "NULL -> " + reactum->to_string();
	return "<error printing rule>";
}

