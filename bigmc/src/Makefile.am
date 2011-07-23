AM_YFLAGS = -y -d

BUILT_SOURCES = bgparser.h

bin_PROGRAMS = bigmc
bigmc_SOURCES = bgparser.ypp \
	bgscanner.ll \
	parser.cpp \
	report.cpp \
	bigraph.cpp \
	driver.cpp \
	graph.cpp \
	hole.cpp \
	main.cpp \
	match.cpp \
	mc.cpp \
	node.cpp \
	parsenode.cpp \
	predicate.cpp \
	query.cpp \
	reactionrule.cpp \
	term.cpp \
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
	../include/term.h

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
	../include/term.h

bigmc_CPPFLAGS = -I../include
includes = ../include
SUBDIRS = predicates


