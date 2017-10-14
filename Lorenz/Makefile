CXX = g++
STD = -std=c++11
LIBS = -lboost_system -lboost_iostreams
EXECS = RKTest Lorenz
RK_OBJS = RKTest.o Analysis/Graph.o Analysis/Stats.o Methods/RK.o Methods/EC.o
LORENZ_OBJS = Lorenz.o Analysis/Graph.o Methods/RK.o Methods/EC.o
CXXFLAGS = $(STD)
LFLAGS = $(STD) $(LIBS)

all : $(EXECS)

RKTest : $(RK_OBJS) RKTest.hpp # All objects are compiled by implicit rules! Hurray for reading the GNU Make manual!
	$(CXX) $(RK_OBJS) -o RKTest $(LFLAGS)

Lorenz : $(LORENZ_OBJS) Lorenz.hpp
	$(CXX) $(LORENZ_OBJS) -o Lorenz $(LFLAGS)

clean :
	rm -f ./*.png $(EXECS) $(RK_OBJS) $(LORENZ_OBJS)
	rm -rf *~