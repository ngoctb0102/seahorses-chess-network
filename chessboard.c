#include<stdio.h>
#include "chessboard.h"
//basic chess board
char *basic = "************************************************123456123456123456123456";
//print order
int print_order[72] = {11,12,13,49,10,14,50,9,15,51,8,16,52,7,17,53,1,2,3,4,5,6,18,19,20,21,22,23,54,48,55,56,57,58,59,60,66,65,64,63,62,61,24,72,47,46,45,44,43,42,30,29,28,27,26,25,71,41,31,70,40,32,69,39,33,68,38,34,67,37,36,35};
//type of block
int type1[10] = {11,10,9,8,7,41,40,39,38,37};
int type2[35] = {12,13,2,3,4,5,6,19,20,21,22,23,25,46,45,44,43,42,29,28,27,26,36,35,56,57,58,59,60,66,65,64,63,62,61};
int type3[10] = {14,15,16,17,18,30,31,32,33,34};
int type4[12] = {49,50,51,52,53,54,72,71,70,69,68,67};
int type5[3] = {1,48,47};
int type6[2] = {55,24};
//block type
int checkBlockType(int blockID){
  for(int i = 0;i<35;i++){
    if(blockID == type2[i]){
      return 2;
    }
    if(i < 12){
      if(blockID == type4[i]){
        return 4;
      }
    }
    if(i < 10){
      if(blockID == type1[i]){
        return 1;
      }
      if(blockID == type3[i]){
        return 3;
      }
    }
    if(i < 3){
      if(blockID == type5[i]){
        return 5;
      }
    }
    if(i < 2){
      if(blockID == type6[i]){
        return 6;
      }
    }
  }
  return 0; //Error !!
}
void printBlock(int blockID, char b){
  int type = checkBlockType(blockID);
  if(type == 1){
    printf("\n                              %c",b);
  }
  if(type == 2){
    printf("     %c",b);
  }
  if(type == 3){
    printf("           %c",b);
  }
  if(type == 4){
    printf("\n                                    %c",b);
  }
  if(type == 5){
    printf("\n%c",b);
  }
  if(type == 6){
    printf("  %c",b);
  }
}
void printChessBoard(char *state){
  for(int i = 0;i<72;i++){
    int order = print_order[i];
    printBlock(order,state[order-1]);
    // printf("%d",print_order[i]);
    // printf("%c",state[i]);
  }
}
void changeState(char *state,int start, int end, char b){
  if(start != 0){
    state[start-1] = basic[start-1];
  }
  state[end-1] = b;
}