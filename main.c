#include<stdio.h>
#include "chessboard.h"

int main(){
  char state[72] = "************************************************123456123456123456123456";
  printChessBoard(state);
  changeState(state,0,1,'R');
  printChessBoard(state);
}