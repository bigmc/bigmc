#include <cpptest.h>

#include <iostream>

#include <bigmc.h>

class MatcherTestSuite : public Test::Suite {
public:
    MatcherTestSuite()
    {
        TEST_ADD(MatcherTestSuite::match_test)
        TEST_ADD(MatcherTestSuite::anywhere_test)
        TEST_ADD(MatcherTestSuite::parallel_test)
    }
    
private:
    void match_test();
    void anywhere_test();
    void parallel_test();
};

void MatcherTestSuite::match_test() {

}

void MatcherTestSuite::parallel_test() {
	control a = bigraph::add_control("a", true, 0);
	control b = bigraph::add_control("b", true, 0);
	control c = bigraph::add_control("c", true, 0);
	control d = bigraph::add_control("d", true, 0);

	set<term *> ch;
	ch.insert(new prefix(a, vector<name>(), new nil()));
	ch.insert(new prefix(b, vector<name>(), new nil()));
	ch.insert(new prefix(c, vector<name>(), new nil()));
	ch.insert(new prefix(d, vector<name>(), new nil()));
	parallel *tm = new parallel(ch);

	set<term *> ch2;
	ch2.insert(new prefix(a, vector<name>(), new nil()));
	ch2.insert(new prefix(b, vector<name>(), new nil()));
	ch2.insert(new hole(0));
	parallel *redex = new parallel(ch2);

	set<match *> matches = matcher::try_match(tm, redex, new match(new reactionrule(redex, new nil())));	

	std::cout << "matches.size(): " << matches.size() << std::endl;

	for(set<match *>::iterator i = matches.begin(); i != matches.end(); i++) {
		std::cout << (*i)->to_string() << endl;
	}

	TEST_ASSERT(matches.size() == 1);
}



void MatcherTestSuite::anywhere_test() {

}

int main(int argc, char **argv) {
	Test::CompilerOutput output(Test::CompilerOutput::GCC);
	MatcherTestSuite ts;
	return ts.run(output) ? EXIT_SUCCESS : EXIT_FAILURE;
}


