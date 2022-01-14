#include<stdio.h>
#include<stdlib.h>
#include "player.h"
#include "horse.h"
#include "game.h"
#include "chessboard.h"
char *basicChess = "************************************************123456123456123456123456";

Game *makeGame(int roomID, int playerNum, char username1[],char username2[], char username3[], char username4[]){
  Game *game = (Game*)malloc(sizeof(Game));
  game->roomID = roomID;
  game->playerNum = playerNum - 1;
  Player *p1 = makePlayer(username1, 0, CHARPRINT[0], 0);
  Player *p2 = makePlayer(username2, 1, CHARPRINT[1], 1);
  game->turn = 0;
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
  strcpy(game->state,basicChess);
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
int checkCanMove(char *state, Horse horse, int move_num){
  int position = horse.position;
  if(position == 0){
    if(move_num > 1){
      return 0;
    }else{
      char pos = state[horse.step[position] - 1];
      if(pos != horse.printChar){
        return 1;
      }else{
        return 0;
      }
    }
  } 
  if(position < 48 && (position + move_num) > 48) return 0;
  for(int i = 1; i < move_num + 1;i++){
	  int block_id = horse.step[position - 1] - 1;
    if(block_id + i > 71) return 0;
	  char pos = state[(block_id + i)%48];
    printf("\n%d-%c\n",block_id + i,pos);
    if(position < 48){
      if(pos != '*' && i < move_num){
        return 0; 
      }
      if(pos == horse.printChar && i == move_num){
        return 0; 
      }
    }
    if(position == 48){
      printf("%c",state[horse.step[position]]);
      if(state[horse.step[position]-1] != '1') return 0;
      else{
        for(int j = 0;j < move_num;j++){
          char poss = state[horse.step[position]-1 +j];
          if(poss == horse.printChar) return 0;
        }
        return 1;
      }
    }
    if(position > 48){
      if(position == 54) return 0;
      if(move_num > 1) return 0;
      else{
        if(state[horse.step[position]-1] == horse.printChar) return 0;
        return 1;
      }
    } 
  }
  return 1;
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
  char kick = game->state[end-1];
  if(kick != '*' && kick != '1' && kick != '2' && kick != '3' && kick != '4' && kick != '5' && kick != '6' ){
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

int moveNum(int pos, int dice){
  int result = dice;
  if(pos == 0){
    if(dice == 1 || dice == 6){
      result = 1;
    }else{
      result = dice;
    }
  }
  if(pos > 48){
    if(dice == pos-48+1){
      result =  1;
    }else{
      if(dice == 1){
        return 2;
      }
      result = dice;
    }
  }
  return result;
}

//0 xuat quan, 1 tien len, 2 len chuong
void getOption(char *finalresult, Game *game, int playerIndex, int dice){
  // char result[13] = "";
  int totalMove = 0;
  for(int i = 0;i < 4; i++){
    // char re[3];
    int pos = game->p[playerIndex].horse[i].position;
    int move_num = moveNum(pos,dice);
    int check = checkCanMove(game->state, game->p[playerIndex].horse[i], move_num);
    if(check == 1){
      if(pos == 0){
        finalresult[2 + 3*totalMove] = '0';
      }else if(pos >= 48){
        finalresult[2 + 3*totalMove] = '2';
      }else{
        finalresult[2 + 3*totalMove] = '1';
      }
      finalresult[1 + 3*totalMove] = i + 48;
      finalresult[3 + 3*totalMove] = move_num + 48;
      totalMove++;
      // strcat(result,re);
    }
  }
  finalresult[0] = totalMove+48;
  if(totalMove == 0){
    finalresult[1] = '0';
    finalresult[2] = '0';
    finalresult[3] = '0';
  }
  // char fi[13] = "";
  // fi[0] = totalMove + 48;
  // strcat(fi,result);
  // strcpy(finalresult, fi);
}

void printGame(Game *game){
  for(int i = 0; i < 4; i++){
    printf("Player %d\n", i);
    for(int j = 0;j < 4;j++){
      printf("Con ngua thu %d: %d\n",j,game->p[i].horse[j].position);
    }
  }
}

int checkEndGame(Game *game){
  int wp = 0;
  for(int i = 0;i<4;i++){
    if(checkWin(game->p[i]) == 1){
      wp+=1;
    }
  }
  return wp;
}

