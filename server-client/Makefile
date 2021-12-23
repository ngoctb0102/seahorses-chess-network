all: server client

server: server.o util.o
	gcc server.o util.o -pthread -o server

server.o: server.c
	gcc -c -Wall server.c

client: client.o
	gcc client.o -o client

client.o: client.c
	gcc -c -Wall client.c

util.o: util.c
	gcc -c -Wall util.c

clean:
	rm -f *.o *~