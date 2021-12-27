all: server client

# ----------------------Server----------------------
server: server.o util.o user.o room.o server_room.o server_user.o
	gcc server.o util.o user.o room.o server_room.o server_user.o -pthread -o server

server.o: server-side/server.c
	gcc -c -Wall server-side/server.c

# ----------------------Client----------------------
client: client.o util.o room.o client_user.o client_home.o
	gcc client.o util.o room.o client_user.o client_home.o -o client

client.o: client-side/client.c
	gcc -c -Wall client-side/client.c

# -------------------Dependencies-------------------
util.o: util.c
	gcc -c -Wall util.c

user.o: ../user/user.c
	gcc -c -Wall ../user/user.c

room.o: ../room/room.c
	gcc -c -Wall ../room/room.c
	
server_room.o: server-side/server_room.c
	gcc -c -Wall server-side/server_room.c

server_user.o: server-side/server_user.c
	gcc -c -Wall server-side/server_user.c

client_user.o: client-side/client_user.c
	gcc -c -Wall client-side/client_user.c

client_home.o: client-side/client_home.c
	gcc -c -Wall client-side/client_home.c

# -------------------------------------------------
# To clean all object file
clean:
	rm -f *.o *~