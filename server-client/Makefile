all: server client

server: server.o util.o user.o room.o
	gcc server.o util.o user.o room.o -pthread -o server

server.o: server.c
	gcc -c -Wall server.c

client: client.o util.o room.o
	gcc client.o util.o room.o -o client

client.o: client.c
	gcc -c -Wall client.c

util.o: util.c
	gcc -c -Wall util.c

user.o: ../user/user.c
	gcc -c -Wall ../user/user.c

room.o: ../room/room.c
	gcc -c -Wall ../room/room.c

clean:
	rm -f *.o *~