enigma: main.o
	g++ -o enigma main.o
main.o: main.cpp
	g++ -c main.cpp
clean:
	rm enigma *.o
