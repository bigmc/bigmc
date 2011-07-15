BIN=../bin
CPP=g++
LIBS=
INCLUDE=-I../include/
BINARY=$(BIN)/bigmc
DEPS=main.o bigraph.o \
	match.o node.o edge.o

all: $(BINARY)

$(BINARY): $(DEPS)
	$(CPP) -o $(BINARY) $(LIBS) $(INCLUDE) $(DEPS)

%.o: %.cpp
	$(CPP) -c $(INCLUDE) -Wall -o $@ $<

clean:
	rm -f *.o $(BINARY)
