#include<stdio.h>
#include<stdlib.h>
#include "player.h"
#include "horse.h"
#include "game.h"
#include "chessboard.h"
char *basic = "************************************************123456123456123456123456";

Game *makeGame(int roomID, int playerNum, char username1[],char username2[], char username3[], char username4[]){
  Game *game = (Game*)malloc(sizeof(Game));
  game->roomID = roomID;
  game->playerNum = playerNum - 1;
  Player *p1 = makePlayer(username1, 0, CHARPRINT[0], 0);
  Player *p2 = makePlayer(username2, 1, CHARPRINT[1], 1);
  game->p[0] = *p1;
  game->p[1] = *p2;
  if(playerNum > 2){
    Player *p3 = makePlayer(username3, 2, CHARPRINT[2], 2);
    game->p[2] = *p3;
  }
  if(playerNum > 3){
    Player *p4 = makePlayer(username4, 3, CHARPRINT[3], 3);
    game->p[3] = *p4;
  }
  strcpy(game->state,basic);
  return game;
}
//get number chess in board
int ChessInBoard(Player *p){
  int total = 0;
  for(int i = 0;i < 4;i++){
    if(p->horse[i].position > 0){
      total += 1;
    }
  }
  return total;
}

int rollDice(){
  int r = 1 + rand() % 6;
  return r;
}
//check horse can move or not
int checkCanMove(char *state, Horse *horse, int move_num){
  int position = horse->position;
  if(position < 48 && (position + move_num) > 48) return 0;
  for(int i = 1; i < move_num + 1;i++){
	  int block_id = horse->step[position - 1] - 1;
	  char pos = state[block_id + i];
    if(position < 48){
      if(pos != '*' && i < move_num){
        return 0; 
      }
      if(pos == horse->printChar && i == move_num){
        return 0; 
      }
      return 1;
    }
    if(position == 48){
      if(state[block_id + 1] != 1) return 0;
      else{
        for(int j = 1;j < move_num+1;j++){
          char pos = state[block_id+j];
          if(pos == horse->printChar) return 0;
        }
        return 1;
      }
    }
    if(position > 48){
      if(position == 54) return 0;
      if(move_num > 1) return 0;
      else{
        if(state[horse->step[position] - 1] == horse->printChar) return 0;
        return 1;
      }
    } 
  }
}
//get player index when meet in state
int playerChar(char c){
  if(c == 'R'){
    return 0;
  }
  if(c == 'G'){
    return 1;
  }
  if(c == 'B'){
    return 2;
  }
  if(c == 'Y'){
    return 3;
  }
}
//update all game after 1 round
Game *updateGame(Game *game, int playerIndex,int horseIndex, int move_num){
  int pos = game->p[playerIndex].horse[horseIndex].position;
  int start = game->p[playerIndex].horse[horseIndex].step[pos - 1];
  game->p[playerIndex].horse[horseIndex] = updatePos(game->p[playerIndex].horse[horseIndex], move_num);
  int end = game->p[playerIndex].horse[horseIndex].step[pos + move_num - 1];
  if(game->state[end-1] != '*'){
    int playerID = playerChar(game->state[end-1]);
    for(int i = 0;i<4;i++){
      int tempPos = game->p[playerID].horse[i].position;
      int check = game->p[playerID].horse[i].step[tempPos - 1];
      if(check == end){
        game->p[playerID].horse[i].position = 0;
      }
    }
  }
  changeState(game->state,start,end,game->p[playerIndex].printChar);
  return game;
}