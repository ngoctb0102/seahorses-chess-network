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

// Room struct
typedef struct Room{
    int room_id;
    Game* game;
    char* players[4];
    int inroom_no;
    RoomStatus status;
} Room;

// Room util

int addRoom(Room** rooms, char* owner);

Room* createRoom(int room_id, char* owner);

Room* createBlankRoom(int room_id);

int addUserToRoom(Room** rooms, int room_id, char* username);

int removeUserFromRoom(Room** rooms, int room_id, char* username);

void delRoom(Room** rooms, int room_id);

// void startGame(Room** rooms, int room_id);

void printRooms(Room** rooms);

void printRoom(Room* room, char* current_user_name);

void freeRoom(Room* node);

char* roomToString(Room** root, int room_id);

#endif