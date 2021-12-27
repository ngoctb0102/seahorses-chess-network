#include<stdio.h>
#include "chessboard.h"
#include "horse.h"
#include "player.h"
#include "game.h"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define RESET "\x1B[0m"

int main(){
  // char state[72] = "************************************************123456123456123456123456";
  // printChessBoard(state);
  // changeState(state,0,1,'R');
  // printChessBoard(state);
  Game *game1 = makeGame(1, 4, "p1","p2", "p3", "p4");
  printChessBoard(game1->state);
  printf("\n");
  printf("\n");
  printf("\n");
  game1 = updateGame(game1,3, 2, 1);
  printGame(game1);
  printChessBoard(game1->state);
  printf("\n");
  printf("\n");
  printf("\n");
  game1 = updateGame(game1,2, 2, 1);
  printGame(game1);
  printChessBoard(game1->state);
  printf("\n");
  printf("\n");
  printf("\n");
  game1 = updateGame(game1,2, 2, 3);
  game1 = updateGame(game1,2, 0, 1);
  printGame(game1);
  printChessBoard(game1->state);
  printf("\n");
  printf("\n");
  printf("\n");
  char mes[20] = "";
  getOption(mes,game1,2,1);
<<<<<<< HEAD
  printf("\n%s\n", mes);
  getOption(mes,game1,1,2);
  printf("\n%s\n", mes);
  getOption(mes,game1,2,6);
  printf("\n%s\n", mes);
  getOption(mes,game1,3,5);
=======
>>>>>>> origin/main
  printf("\n%s\n", mes);
  return 0;

}

