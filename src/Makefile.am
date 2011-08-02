AM_YFLAGS = -y -d

BUILT_SOURCES = bgparser.h

bin_PROGRAMS = bigmc

noinst_LIBRARIES = libbigmc.a

libbigmc_a_SOURCES = bgparser.ypp \
	bgscanner.ll \
	parser.cpp \
	report.cpp \
	bigraph.cpp \
	driver.cpp \
	graph.cpp \
	match.cpp \
	mc.cpp \
	node.cpp \
	globals.cpp \
	parsenode.cpp \
	predicate.cpp \
	query.cpp \
	reactionrule.cpp \
	term.cpp \
	subtree.cpp \
	matcher.cpp \
	terms/regions.cpp \
	terms/parallel.cpp \
	terms/prefix.cpp \
	terms/hole.cpp \
	terms/nil.cpp \
	../include/bigmc.h \
	../include/bigraph.h \
	../include/driver.h \
	../include/graph.h \
	../include/match.h \
	../include/mc.h \
	../include/node.h \
	../include/parsenode.h \
	../include/parser.h \
	../include/predicate.h \
	../include/query.h \
	../include/reactionrule.h \
	../include/report.h \
	../include/globals.h \
	../include/term.h \
	../include/matcher.h \
	../include/subtree.h


noinst_HEADERS = ../include/bigmc.h \
	../include/bigraph.h \
	../include/driver.h \
	../include/graph.h \
	../include/match.h \
	../include/mc.h \
	../include/node.h \
	../include/parsenode.h \
	../include/parser.h \
	../include/predicate.h \
	../include/query.h \
	../include/reactionrule.h \
	../include/globals.h \
	../include/term.h \
	../include/matcher.h \
	../include/subtree.h

libbigmc_a_CPPFLAGS = -I../include
bigmc_CPPFLAGS = -I../include
bigmc_SOURCES = main.cpp
bigmc_LDADD = libbigmc.a

includes = ../include
SUBDIRS = predicates

