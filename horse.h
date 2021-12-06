#ifndef __HORSE_H__
#define __HORSE_H__

typedef struct {
  char printChar;
  int color;
  int position;
  int step[54];
}Horse;

Horse *makeHorse(char printChar, int color, int setStep);
Horse *updatePos(Horse horse,int move_num)

#endif