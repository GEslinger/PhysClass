CXX = g++
STD = -std=c++11
LIBS = -lboost_system -lboost_iostreams
EXECS = Simulate
OBJS = Planet.o Simulate.o Graph.o Calcs.o
CXXFLAGS = $(STD)
LFLAGS = $(STD) $(LIBS)

all : $(EXECS)

Simulate : $(OBJS) Simulate.hpp
	$(CXX) $(OBJS) -o Simulate $(LFLAGS)

clean :
	rm -f ./*.png $(EXECS) $(OBJS)
	rm -rf *~