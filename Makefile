project.out: kd.o oct.o main.o
	g++ -std=c++11 -stdlib=libc++ main.o kd.o oct.o

main.o: main.cpp
	g++ -std=c++1y -stdlib=libc++ -c main.cpp

kd.o: kd.cpp
	g++ -std=c++1y -stdlib=libc++ -c kd.cpp

oct.o: oct.cpp
	g++ -std=c++1y -stdlib=libc++ -c oct.cpp
