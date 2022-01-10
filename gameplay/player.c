#include<stdio.h>
#include<stdlib.h>
#include "horse.h"
#include "player.h"
//tao ra nguoi choi
Player *makePlayer(char username[], int playOrder,char printChar, int color){
  Player *player = (Player*)malloc(sizeof(Player));
  strcpy(player->username,username);
  player->playOrder = playOrder;
  player->printChar = printChar;
  Horse *horse1 = makeHorse(printChar,color,playOrder);
  Horse *horse2 = makeHorse(printChar,color,playOrder);
  Horse *horse3 = makeHorse(printChar,color,playOrder);
  Horse *horse4 = makeHorse(printChar,color,playOrder);
  player->horse[0] = *horse1;
  player->horse[1] = *horse2;
  player->horse[2] = *horse3;
  player->horse[3] = *horse4;
  return player;
}
// tinh tong position
int getTotalPos(Player p){
  int sum = 0;
  for(int i = 0;i<4;i++){
    sum += p.horse[i].position;
  }
  return sum;
}
//kiem tra dieu kien thang
int checkWin(Player p){
  int sum = getTotalPos(p);
  if(sum == 210){
    return 1;
  }
  return 0;
}