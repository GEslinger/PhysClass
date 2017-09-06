CC = g++
STD = -std=c++11
LIBS = -lboost_system -lboost_iostreams
OBJS = SHMMethods.o
CFLAGS = -c $(STD)
LFLAGS = $(STD) $(LIBS)

main : $(OBJS)
	$(CC) $(OBJS) -o main $(LFLAGS) 

SHMMethods.o : Tools.hpp SHMMethods.cpp
	$(CC) SHMMethods.cpp $(CFLAGS)

clean :
	rm -f *.o *~ ./*.png main