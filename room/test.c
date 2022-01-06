#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"

Room* createJoinRoom(int room_id, int inroom_no, char** inroom_players){
    Room* joinroom = (Room*) malloc(sizeof(Room));
    joinroom->game = NULL;
    joinroom->inroom_no = inroom_no + 1;
    joinroom->room_id = room_id;
    joinroom->status = WAITING;
    int i = 0;
    for(; i < inroom_no; i++){
        joinroom->players[i] = (char*) malloc(100);
        strcpy(joinroom->players[i], inroom_players[i]);
    }
    joinroom->players[i] = (char*) malloc(100);
    strcpy(joinroom->players[i++], "current_user");
    for(; i < MAX_PLAYER_PER_ROOM; i++){
        joinroom->players[i] = (char*) malloc(100);
    }
    return joinroom;
}

void main(){
    // Room* rooms[MAX_ROOM_ALLOWED];
    // for(int i = 0; i < MAX_ROOM_ALLOWED; i++)
    //     rooms[i] = NULL;
    // addRoom(rooms, "long");
    // addRoom(rooms, "tuanvu");
    // addRoom(rooms, "ngocprof");
    // printRooms(rooms);
    // puts("\n=====================");
    // addUserToRoom(rooms, 2, "hieu");
    // printRooms(rooms);
    // puts("\n=====================");
    // removeUserFromRoom(rooms, 1, "tuanvu");
    // printRooms(rooms);
    Room* room = NULL;
    char* names[100] = {"long", "tuanvu", "ngoc"};
    room = createJoinRoom(1, 3, names);
    printRoom(room);
}