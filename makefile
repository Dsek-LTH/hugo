libhugo.a: hugo.o
	ar rcs libhugo.a hugo.o

hugo.o: hugo.c
	gcc -O3 -lwiringPi -c -Wall hugo.c

.PHONY: clean

clean:
	rm *.a *.o

