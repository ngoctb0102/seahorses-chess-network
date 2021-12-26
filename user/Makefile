all: test

test: test.o user.o
	gcc test.o user.o -o test

test.o: test.c
	gcc -c -Wall test.c

user.o: user.c
	gcc -c -Wall user.c

clean:
	rm -f *.o *~