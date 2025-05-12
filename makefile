libhugo/libhugo.a: libhugo/hugo.o
	ar rcs libhugo/libhugo.a libhugo/hugo.o

libhugo/hugo.o: libhugo/hugo.c
	gcc -O3 -lwiringPi -c -Wall -o libhugo/hugo.o libhugo/hugo.c

#-------------

libhugo.a: libhugo/libhugo.a
	cp libhugo/libhugo.a libhugo.a
hugo.h: libhugo/hugo.h
	cp libhugo/hugo.h hugo.h

#-------------

life: life.c hugo.h libhugo.a
	gcc -O3 life.c -L. -lhugo -lwiringPi -o life

radar: radar.c hugo.h libhugo.a
	gcc -O3 radar.c -L. -lhugo -lwiringPi -o radar


udp: udp.c hugo.h libhugo.a
	gcc -O3 udp.c -L. -lhugo -lwiringPi -o udp

#-------------

all:
	make life
	make radar
	make udp


.PHONY: clean
clean:
	rm -f *.a *.o
	rm -f hugo.h
	rm -f libhugo.a
	cd libhugo && make clean
