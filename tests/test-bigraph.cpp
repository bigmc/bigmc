#include <cpptest.h>

#include <bigmc.h>

class BigraphTestSuite : public Test::Suite {
public:
    BigraphTestSuite()
    {
        TEST_ADD(BigraphTestSuite::name_test)
        TEST_ADD(BigraphTestSuite::control_test)
    }
    
private:
    void name_test();
    void control_test();
};

void BigraphTestSuite::name_test() {
	bigraph *b = new bigraph(1);

	name n1 = bigraph::name_from_string("abc");
	name n2 = bigraph::name_from_string("a");
	name n3 = bigraph::name_from_string("Xyz");

	b->add_outer_name(n1);
	b->add_outer_name(n2);

	TEST_ASSERT(bigraph::name_to_string(n1) == "abc");
	TEST_ASSERT(!bigraph::is_free(n2));
	TEST_ASSERT(bigraph::is_free(n3));

	delete b;
}

void BigraphTestSuite::control_test() {
	control c1 = bigraph::control_from_string("abcdefg");
	control c2 = bigraph::control_from_string("Xyz");
	control c3 = bigraph::control_from_string("FOO");
	
	TEST_ASSERT(bigraph::control_to_string(c1) == "abcdefg");
	TEST_ASSERT(bigraph::control_to_string(c2) == "Xyz");
	TEST_ASSERT(bigraph::control_to_string(c3) == "FOO");

	bigraph *b = new bigraph(0);

	bigraph::add_control("abcdefg", true, 2);
	bigraph::add_control("Xyz", false, 0);

	TEST_ASSERT(bigraph::arity(c1) == 2);
	TEST_ASSERT(bigraph::arity(c2) == 0);
	
	TEST_ASSERT(bigraph::activity(c1));
	TEST_ASSERT(!bigraph::activity(c2));

	delete b;
}

int main(int argc, char **argv) {
	Test::CompilerOutput output(Test::CompilerOutput::GCC);
	BigraphTestSuite ts;
	return ts.run(output) ? EXIT_SUCCESS : EXIT_FAILURE;
}

