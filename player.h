#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "horse.h"

typedef struct {
  char username[100];
  int playOrder;
  Horse horse[4];
  char printChar;
}Player;

Player *makePlayer(char username[], int playOrder, char printChar, int color);
int getTotalPos(Player p);
int checkWin(Player p);

#endif