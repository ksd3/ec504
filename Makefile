main.o: main.cpp
	g++ -std=c++1y -c main.cpp

kd.o: kd.cpp
	g++ -std=c++1y -c kd.cpp

oct.o: oct.cpp
	g++ -std=c++1y -c oct.cpp

project.out: kd.o oct.o main.o
	g++ -std=c++1y main.o kd.o oct.o
