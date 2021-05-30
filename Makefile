PROG=tsp
SRCS=main.cc

CXX=g++ -std=c++11 -O3

$(PROG): main.cc
	$(CXX) main.cc -o tsp

clean:
	rm -f $(PROG)
