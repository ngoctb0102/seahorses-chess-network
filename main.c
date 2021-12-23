#include<stdio.h>
#include "chessboard.h"
#include "horse.h"
#include "player.h"
#include "game.h"

int main(){
  // char state[72] = "************************************************123456123456123456123456";
  // printChessBoard(state);
  // changeState(state,0,1,'R');
  // printChessBoard(state);
  Game *game1 = makeGame(1, 4, "p1","p2", "p3", "p4");
  printChessBoard(game1->state);
  printf("\n");
  game1 = updateGame(game1,3, 2, 5);
  printChessBoard(game1->state);
  printf("\n");
  game1 = updateGame(game1,2, 2, 1);
  printChessBoard(game1->state);
  return 0;

}

