#ifndef __SERVEROOM_H__
#define __SERVEROOM_H__

#include "../../room/room.h"
#include "../../user/user.h"

// This file define server action when there is room-related request

// Global variable
extern int current_no_room;
extern Room* rooms[MAX_ROOM_ALLOWED];

// Functions

void userCreateRoom(char** msg, UserNode** current_user);

void userExitRoom(char** msg, UserNode** current_user);

void userJoinRoom(char** msg, UserNode** current_user);

#endif