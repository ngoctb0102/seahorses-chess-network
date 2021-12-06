#include<stdio.h>
#include<string.h>

#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__

int checkBlockType(int blockID);
void printBlock(int blockID, char b);
void printChessBoard(char *state);

#endif