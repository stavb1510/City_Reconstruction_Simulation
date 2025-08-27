# Please implement your Makefile rules and targets below.

all: clean compile link cleanObj

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp

link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/simulation *.o

clean:
	rm -f bin/*

cleanObj:
	rm -f *.o