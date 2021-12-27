#ifndef __ROOM_H__
#define __ROOM_H__

#include "../gameplay/game.h"
#include "../gameplay/player.h"

#define MAX_PLAYER_PER_ROOM 4
#define MAX_ROOM_ALLOWED 6

// Room status
typedef enum {
    WAITING,
    PLAYING
} RoomStatus;

// RoomNode in room bst
typedef struct Room{
    int room_id;
    Game* game;
    char* players[4];
    int inroom_no;
    RoomStatus status;
} Room;

// Room BST util

int addRoom(Room** rooms, char* owner);

Room* createRoom(int room_id, char* owner);

int addUserToRoom(Room** rooms, int room_id, char* username);

void removeUserFromRoom(Room** rooms, int room_id, char* username);

void delRoom(Room** rooms, int room_id);

void startGame(Room** rooms, int room_id);

void printRooms(Room** rooms);

void printRoom(Room* room);

void freeRoom(Room* node);

#endif