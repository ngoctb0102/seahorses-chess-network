all: test

test: main.o game.o chessboard.o horse.o player.o
	gcc main.o game.o chessboard.o horse.o player.o -o test

horse.o: horse.c
	gcc -c -Wall horse.c

player.o: player.c
	gcc -c -Wall player.c

game.o: game.c
	gcc -c -Wall game.c

chessboard.o: chessboard.c
	gcc -c -Wall chessboard.c

main.o: main.c
	gcc -c -Wall main.c

clean:
	rm -f *.o *~