CC = g++
STD = -std=c++11
LIBS = -lboost_system -lboost_iostreams
OBJS = Magnet.o Ising.o
CFLAGS = -c $(STD)
LFLAGS = $(STD) $(LIBS)

main : $(OBJS)
	$(CC) $(OBJS) -o main $(LFLAGS) 

Magnet.o : Magnet.hpp Magnet.cpp
	$(CC) Magnet.cpp $(CFLAGS)

Ising.o : Tools.hpp Ising.cpp Magnet.hpp Tools.hpp
	$(CC) Ising.cpp $(CFLAGS)

clean :
	rm -f *.o *~ ./*.png main