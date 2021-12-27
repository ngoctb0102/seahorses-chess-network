#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"

void main(){
    Room* rooms[MAX_ROOM_ALLOWED];
    for(int i = 0; i < MAX_ROOM_ALLOWED; i++)
        rooms[i] = NULL;
    addRoom(rooms, "long");
    addRoom(rooms, "tuanvu");
    addRoom(rooms, "ngocprof");
    printRooms(rooms);
    puts("\n=====================");
    addUserToRoom(rooms, 2, "hieu");
    printRooms(rooms);
    puts("\n=====================");
    removeUserFromRoom(rooms, 1, "tuanvu");
    printRooms(rooms);
}