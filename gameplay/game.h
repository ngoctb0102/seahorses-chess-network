#ifndef __GAME_H__
#define __GAME_H__

#include "horse.h"
#include "player.h"
#define CHARPRINT "RGBY"

typedef struct{
  int roomID;
  Player p[4];
  char state[72];
  int playerNum;
}Game;

Game *makeGame(int roomID, int playerNum, char username1[],char username2[], char username3[], char username4[]);
Game *updateGame(Game *game, int playerIndex,int horseIndex, int move_num);
int playerChar(char c);
int checkCanMove(char *state, Horse horse, int move_num);
int rollDice();
int ChessInBoard(Player *p);
void getOption(char *finalresult, Game *game, int playerIndex, int dice);
int moveNum(int pos, int dice);
void printGame(Game *game);

#endif