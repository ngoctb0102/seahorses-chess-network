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
typedef struct RoomNode{
    int room_id;
    Game* game;
    char* players[4];
    int inroom_no;
    RoomStatus status;
    struct RoomNode *right, *left;
} RoomNode;

// Room BST util
RoomNode* addRoom(RoomNode* root, int room_id, char* owner);
RoomNode* createRoom(int room_id, char* owner);
RoomNode* searchRoom(RoomNode* root, int room_id);
int addUserToRoom(RoomNode* root, int room_id, char* username);
void removeUserFromRoom(RoomNode** root, int room_id, char* username);
void delRoom(RoomNode** root, int room_id);
void startGame(RoomNode* root, int room_id);
void printRoom(RoomNode* room);
void printRoomTree(RoomNode* root);


#endif