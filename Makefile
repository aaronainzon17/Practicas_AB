PROG=tsp
SRCS=main.cc

CXX=g++ -std=c++11 -O3

OBJS:=$(SRCS:.cc=.o)
DEPS:=$(SRCS:.cc=.d)

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cc
	$(CXX) -MMD -c $<

clean:
	@rm -f $(PROG) *.o *.d core

-include $(DEPS)
