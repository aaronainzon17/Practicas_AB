PROG=tsp
SRCS=main.cc

CXX=g++ -std=c++11 -O3

all: $(PROG)

$(PROG): main.cc
	$(CXX) main.cc -o tsp

clean:
	rm -f $(PROG)
