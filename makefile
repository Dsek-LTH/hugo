hugo.o: hugo.c
	gcc -O3 -lwiringPi -c -Wall hugo.c

libhugo.a: hugo.o
	ar rcs libhugo.a hugo.o

