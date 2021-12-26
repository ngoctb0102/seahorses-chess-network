all: test

test: test.o room.o
	gcc test.o room.o -o test

test.o: test.c
	gcc -c -Wall test.c

room.o: room.c
	gcc -c -Wall room.c

clean:
	rm -f *.o *~