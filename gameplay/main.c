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
  Game *game = makeGame(1, 4, "p1","p2", "p3", "p4");
  int turn = 0;
  while(checkEndGame(game) != 3){
    int pid = turn%4;
    if(checkWin(game->p[pid]) == 1){
      turn+=1;
      continue;
    }
    //doan nay server -> client
    printf("\nNguoi choi %d: hay tung xuc sac !\n",pid);
    int dice;
    int op;
    scanf("%d",&op);
    // doan nay rollDice r gui ve
    if(op != 1){
      printf("PASS\n");
      turn+=1;
      continue;
    }else{
      dice = rollDice();
      printf("nguoi choi %d duoc %d\n",pid,dice);
    }
    char option[20];
    getOption(option,game,pid,dice);
    //server gui option cho client
    if((option[0] - 48) == 0){
      printf("ban khong co con ngua nao di chuyen dduoc\n");
      turn+=1;
      continue;
    }
    for(int i =0;i < option[0] - 48;i++){
      printf("%d: ",i);
      if(option[3*i + 2]-48 == 0){
        printf("Xuat quan thu %d\n",i+1);
      }else if(option[3*i + 2]-48 == 1){
        printf("Quan thu %d tien len %d\n",i+1,option[3*i + 3]-48);
      }else{
        printf("Quan thu %d len chuong\n",i+1);
      }
    }
    printf("Moi ban lua chon: ");
    int opt;
    scanf("%d",&opt);
    //gui lua chon cua client ve server
    game = updateGame(game, pid,option[3*opt+1]-48 ,option[3*opt + 3]-48);//updateGame
    for(int i = 0;i<4;i++){
      if(checkWin(game->p[i]) == 1){
        printf("player %d win\n", i);
      }
    }
    //gui tra lai state cho player
    printChessBoard(game->state);
    turn+=1;
  }
  return 0;
}

