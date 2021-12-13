CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: test

test: main.o game.o chessboard.o horse.o player.o
  ${CC} main.o game.o chessboard.o horse.o player.o -o test

main.o: main.c
  ${CC} ${CFLAGS} main.c

game.o: game.c
  ${CC} ${CFLAGS} game.c

chessboard.o: chessboard.c
  ${CC} ${CFLAGS} chessboard.c

horse.o: horse.c
  ${CC} ${CFLAGS} horse.c

player.o: player.c
  ${CC} ${CFLAGS} player.c

clean:
	rm -f *.o *~