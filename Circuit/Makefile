CXX = g++
STD = -std=c++11
LIBS = -lboost_system -lboost_iostreams
EXECS = Scrolls Bifs
SOBJS = Circuit.o Graphing.o Scrolls.o
BOBJS = Circuit.o Graphing.o Bifs.o
CXXFLAGS = $(STD)
LFLAGS = $(STD) $(LIBS)

all : $(EXECS)

Scrolls : $(SOBJS) Circuit.hpp
	$(CXX) $(SOBJS) -o Scrolls $(LFLAGS)

Bifs : $(BOBJS) Bifs.hpp
	$(CXX) $(BOBJS) -o Bifs $(LFLAGS)

clean :
	rm -f ./*.png $(EXECS) $(SOBJS) $(BOBJS)
	rm -rf *~