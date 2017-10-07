CC = g++
STD = -std=c++11
LIBS = -lboost_system -lboost_iostreams
OBJS = RKTest 
CFLAGS = -c $(STD)
LFLAGS = $(STD) $(LIBS)

all : $(OBJS)

RKTest : RKTest.o Graph.o RK.o EC.o
	$(CC) RKTest.o Graph.o RK.o EC.o -o RKTest $(LFLAGS) 

RKTest.o : Method.h
	$(CC) RKTest.cpp $(CFLAGS)

Graph.o : Graph.cpp
	$(CC) Graph.cpp $(CFLAGS)

RK.o : RK.cpp Method.h
	$(CC) RK.cpp $(CFLAGS)

EC.o : EC.cpp Method.h
	$(CC) EC.cpp $(CFLAGS)

clean :
	rm -f *.o *~ ./*.png $(OBJS)