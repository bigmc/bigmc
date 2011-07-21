CPP=g++ -I../../include -Wall
CXX=g++ -I../../include -Wall
PEG=../../lib/peg-0.1.4/peg
LEG=../../lib/peg-0.1.4/leg
LIBS=
INCLUDE=-I../../include/ -I/opt/local/include/
DEPS=parser.o lex.yy.o bgparser.tab.o

all: $(DEPS)

bgparser.tab.o: bgparser.tab.cpp

bgparser.tab.cpp: bgparser.ypp
	bison -d -o bgparser.tab.cpp bgparser.ypp

parser.o: parser.cpp
	$(CPP) -c $(INCLUDE) -Wall -o parser.o parser.cpp

lex.yy.o: lex.yy.cc

lex.yy.cc: bgscanner.l bgparser.tab.o
	flex -o lex.yy.cc bgscanner.l

clean:
	rm -f *.o bgparser.h bgparser.tab.cpp lex.yy.cc

