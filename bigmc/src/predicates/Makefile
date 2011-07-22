CPP=g++ -I../../include -Wall
LIBS=
INCLUDE=-I../../include/
LIBDIR=../../lib
DEPS=$(LIBDIR)/pred_equal.so \
     $(LIBDIR)/pred_empty.so \
     $(LIBDIR)/pred_terminal_contains.so

all: $(DEPS)

$(LIBDIR)/%.so: %.cpp
	g++ -Wall -dynamiclib -fPIC $(INCLUDE) -flat_namespace -undefined dynamic_lookup -o $@ $<

clean:
	rm -f *.o $(DEPS) 



