all: server client

# ----------------------Server----------------------
server: server.o util.o user.o room.o server_room.o server_user.o game.o chessboard.o horse.o player.o
	gcc server.o util.o user.o room.o server_room.o server_user.o game.o chessboard.o horse.o player.o -pthread -o server

server.o: server-client/server-side/server.c
	gcc -c -Wall server-client/server-side/server.c

# ----------------------Client----------------------
client: client.o util.o user.o room.o client_user.o client_home.o client_game.o chessboard.o game.o horse.o player.o
	gcc client.o util.o user.o room.o client_user.o client_home.o client_game.o chessboard.o game.o horse.o player.o -pthread -o client

client.o: server-client/client-side/client.c
	gcc -c -Wall server-client/client-side/client.c

# -------------------Dependencies-------------------
util.o: server-client/util.c
	gcc -c -Wall server-client/util.c

user.o: user/user.c
	gcc -c -Wall user/user.c

room.o: room/room.c
	gcc -c -Wall room/room.c

game.o: gameplay/game.c
	gcc -c -Wall gameplay/game.c
	
server_room.o: server-client/server-side/server_room.c
	gcc -c -Wall server-client/server-side/server_room.c

server_user.o: server-client/server-side/server_user.c
	gcc -c -Wall server-client/server-side/server_user.c

client_user.o: server-client/client-side/client_user.c
	gcc -c -Wall server-client/client-side/client_user.c

client_home.o: server-client/client-side/client_home.c
	gcc -c -Wall server-client/client-side/client_home.c

client_game.o: server-client/client-side/client_game.c
	gcc -c -Wall server-client/client-side/client_game.c

horse.o: gameplay/horse.c
	gcc -c -Wall gameplay/horse.c

player.o: gameplay/player.c
	gcc -c -Wall gameplay/player.c

chessboard.o: gameplay/chessboard.c
	gcc -c -Wall gameplay/chessboard.c

# -------------------------------------------------
# To clean all object file
clean:
	rm -f *.o *~