
AM_DEFAULT_SOURCE_EXT = .cpp
AM_COLOR_TESTS=yes
AM_LDFLAGS = -lcpptest

check_PROGRAMS = test-bigraph test-matcher

TESTS = $(check_PROGRAMS)

test_bigraph_SOURCES = test-bigraph.cpp
test_bigraph_CPPFLAGS = -I../include
test_bigraph_LDADD = $(top_builddir)/src/libbigmc.a

test_matcher_SOURCES = test-matcher.cpp
test_matcher_CPPFLAGS = -I../include
test_matcher_LDADD = $(top_builddir)/src/libbigmc.a

