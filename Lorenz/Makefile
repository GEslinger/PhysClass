CXX = g++
STD = -std=c++11
LIBS = -lboost_system -lboost_iostreams
EXECS = RKTest 
RK_OBJS = RKTest.o Analysis/Graph.o Methods/RK.o Methods/EC.o
CXXFLAGS = $(STD) -c
LFLAGS = $(STD) $(LIBS)

all : $(EXECS)

RKTest : $(RK_OBJS) # All objects are compiled by implicit rules! Hurray for reading the GNU Make manual!
	$(CXX) $(RK_OBJS) -o RKTest $(LFLAGS)

clean :
	rm -f ./*.png $(EXECS)
	rm -rf $(RK_OBJS) *~