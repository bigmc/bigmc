BIN=../bin
CPP=g++
PEG=../lib/peg-0.1.4/peg
LIBS=-L/opt/local/lib -lreadline
INCLUDE=-I../include/ -I/opt/local/include/
BINARY=$(BIN)/bigmc
DEPS=main.o bigraph.o \
	match.o edge.o \
	parsenode.o driver.o mc.o \
	reactionrule.o term.o graph.o \
	node.o query.o

PARSERDEPS=parser/parser.o parser/bgparser.tab.o parser/lex.yy.o

.PHONY: parser clean

all: $(BINARY)

$(BINARY): $(DEPS) parser
	$(CPP) -o $(BINARY) $(LIBS) $(INCLUDE) $(DEPS) $(PARSERDEPS)

parser:
	make -C parser

%.o: %.cpp
	$(CPP) -c $(INCLUDE) -Wall -o $@ $<

clean:
	rm -f *.o $(BINARY)
	make -C parser clean
