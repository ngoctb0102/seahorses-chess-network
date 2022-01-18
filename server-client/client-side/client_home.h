#ifndef __CLIENTHOME_H__
#define __CLIENTHOME_H__

#include "../../room/room.h"

//----------Globals-----------
extern Room* my_room;

//-------Functions-----------

void requestCreateRoom(int sock);

int requestJoinRoom();

void requestFindRoom();

void exitRoom(int sock);

Room* createJoinRoom(char** msg);

#endif