CC = /usr/local/bin/gcc
CXX = /usr/local/bin/g++
ALLOBJ = Graph.o Exceptions.o main.o Shell.o
PYOBJS = graph.o Graph.o Exceptions.o
EXEC = gcalc
DEBUG_FLAG = -DNDEBUG
COMP_FLAG = -std=c++11 -Wall -Werror -pedantic-errors -fPIC 

$(EXEC): $(ALLOBJ)
	$(CXX) $(DEBUG_FLAG) $(ALLOBJ) -o $@

Graph.o: Graph.cpp Graph.h Exceptions.h
	$(CXX) -g -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

main.o: main.cpp Graph.h Exceptions.h Shell.h
	$(CXX) -g -c -g $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

Shell.o: Shell.cpp Shell.h Graph.h Exceptions.h
	$(CXX) -g -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

Exceptions.o: Exceptions.cpp Exceptions.h
	$(CXX) -g -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

graph.o: graph.cpp graph.h
	$(CXX) -g -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

clean:
	rm -f Exceptions.o Graph.o main.o Shell.o graph.o gcalc libgraph.a

tar:
	zip gcalc.zip Exceptions.h Exceptions.cpp Graph.h Graph.cpp main.cpp Shell.h Shell.cpp graph.cpp graph.h graph.i design.pdf Makefile test_in.txt test_out.txt

libgraph.a: $(PYOBJS)
	ar -rs $@ $^