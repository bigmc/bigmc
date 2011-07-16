CPP=g++ -I../../include -Wall
CXX=g++ -I../../include -Wall
PEG=../../lib/peg-0.1.4/peg
LEG=../../lib/peg-0.1.4/leg
LIBS=
INCLUDE=-I../../include/ -I/opt/local/include/
DEPS=parser.o bgparser.leg.o

all: $(DEPS)

bgparser.leg.o: bgparser.leg.cpp

bgparser.leg.cpp: bg.leg
	$(LEG) -o bgparser.leg.cpp bg.leg

parser.o: parser.cpp bgparser.leg.cpp
	$(CPP) -c $(INCLUDE) -Wall -o parser.o parser.cpp

clean:
	rm -f *.o bgparser.leg.cpp 

