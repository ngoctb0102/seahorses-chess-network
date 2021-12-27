#ifndef __SERVEROOM_H__
#define __SERVEROOM_H__

#include "../../room/room.h"

// This file define server action when there is room-related request

// Global variable
extern int current_no_room;
extern Room* rooms[MAX_ROOM_ALLOWED];

// Functions

void userCreateRoom(char** msg, int sock);

void userExitRoom(char** msg, int sock);

void userJoinRoom(char** msg, int sock);

#endif