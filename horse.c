#include<stdio.h>
#include "horse.h"
//buoc di co dinh cua moi con ngua
int step1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48}
int step2 = {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
int step3 = {25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24}
int step4 = {37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36}
//khoi tao ngua
Horse *makeHorse(char printChar, int color, int setStep){
  Horse *horse = (Horse*)malloc(sizeof(Horse));
  horse->color = color;
  horse->printChar = printChar;
  horse->position = 0;
  if(setStep == 0){
    horse->step = step1;
  }
  if(setStep == 1){
    horse->step = step2;
  }
  if(setStep == 2){
    horse->step = step3;
  }
  if(setStep == 3){
    horse->step = step4;
  }
  return horse;
}
//cap nhat buoc di
Horse *updatePos(Horse horse,int move_num){
  if(horse->position == 0){
    horse->position = 1;
  }else{
    horse->position += move_num;
  }
  return horse;
}

