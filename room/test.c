#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"

void main(){
    int i = 0;
    RoomNode* root = NULL;
    root = addRoom(root, i++, "long");
    addUserToRoom(root, 0, "longhoang");
    root = addRoom(root, i++, "tuanvu");
    root = addRoom(root, i++, "profngoc");
    root = addRoom(root, i++, "abc");
    addUserToRoom(root, 3, "xxx");
    addUserToRoom(root, 0, "blank");
    printRoomTree(root);
    startGame(root, 0);
    startGame(root, 3);
    printRoomTree(root);
    printf("remove longhoang in room 0\n");
    removeUserFromRoom(&root, 0, "longhoang");
    printRoomTree(root);
    printf("del room 2\n");
    delRoom(&root, 2);
    printRoomTree(root);
    printf("remove long (roomaster) from room 0\n");
    removeUserFromRoom(&root, 0, "long");
    printRoomTree(root);
}