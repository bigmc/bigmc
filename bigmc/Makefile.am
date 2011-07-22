AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = bigmc
bigmc_SOURCES = 
	src/bigraph.cpp \
	src/driver.cpp \
	src/edge.cpp \
	src/graph.cpp \
	src/hole.cpp \
	src/main.cpp \
	src/match.cpp \
	src/mc.cpp \
	src/node.cpp \
	src/parsenode.cpp \
	src/parser/parser.cpp \
	src/predicate.cpp \
	src/predicates/pred_empty.cpp \
	src/predicates/pred_equal.cpp \
	src/predicates/pred_terminal_contains.cpp \
	src/query.cpp \
	src/reactionrule.cpp \
	src/term.cpp \
	include/bigmc.h \
	include/bigraph.h \
	include/driver.h \
	include/edge.h \
	include/graph.h \
	include/match.h \
	include/mc.h \
	include/node.h \
	include/parsenode.h \
	include/parser.h \
	include/predicate.h \
	include/query.h \
	include/reactionrule.h \
	include/term.h \
	src/parser/bgparser.ypp \
	src/parser/bgscanner.l

dist_noinst_SCRIPTS = autogen.sh
