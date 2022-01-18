#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "room.h"

Room* createRoom(int room_id, char* owner){
    // Room* newroom = (Room*) malloc(sizeof(Room));
    // newroom->game = NULL;
    // newroom->room_id = room_id;
    // newroom->inroom_no = 1;
    // newroom->status = WAITING;
    // for(int i = 0; i < MAX_PLAYER_PER_ROOM; i++){
    //     newroom->players[i] = (char*) malloc(100);
    // }
    Room* newroom = createBlankRoom(room_id);
    newroom->status = WAITING;
    newroom->inroom_no = 1;
    strcpy(newroom->players[0], owner);
    return newroom;
}

Room* createBlankRoom(int room_id){
    Room* newroom = (Room*) malloc(sizeof(Room));
    newroom->room_id = room_id;
    newroom->game = NULL;
    newroom->inroom_no = 0;
    for(int i = 0; i < MAX_PLAYER_PER_ROOM; i++){
        newroom->players[i] = (char*) malloc(100);
    }
    return newroom;
}

int addRoom(Room** root, char* owner){
    for(int i = 0; i < MAX_ROOM_ALLOWED; i++){
        if(root[i] == NULL){
            root[i] = createRoom(i, owner);
            return i;
        }
    }
    return -1;
}

int addUserToRoom(Room** root, int room_id, char* username){
    if(root == NULL) return -1;
    Room* node = root[room_id];
    if(node != NULL){
        if(node->inroom_no == MAX_PLAYER_PER_ROOM)
            return -1;
        else{
            strcpy(node->players[node->inroom_no], username);
            node->inroom_no += 1;
            return 1;
        }
    }
    return 0;
}

int removeUserFromRoom(Room** root, int room_id, char* username){
    Room* node = root[room_id];
    if(node == NULL) return -1;
    if(strcmp(node->players[0], username) == 0){
        delRoom(root, room_id);
        return 0;
    }
    int i = 1;
    while(strcmp(node->players[i], username) != 0) i++;
    while(i < node->inroom_no-1) {
        strcpy(node->players[i], node->players[i+1]);
        i++;
    }
    node->inroom_no -= 1;
    return 1;
}

void freeRoom(Room* node){
    for(int i = 0; i < MAX_PLAYER_PER_ROOM; i++){
        free(node->players[i]);
    }
    free(node);
}

void delRoom(Room** root, int room_id){
    // TODO
    Room* node = root[room_id];
    if(node == NULL) return;
    freeRoom(node);
    root[room_id] = NULL;
}

// void startGame(Room** root, int room_id){
//     // TODO
//     if(root[room_id] == NULL) return;
//     root[room_id]->status = PLAYING;
// }

void printRooms(Room** rooms){
    for(int i = 0; i < MAX_ROOM_ALLOWED; i++){
        if(rooms[i] != NULL){
            printRoom(rooms[i], NULL);
        }
    }
}

void printRoom(Room* room, char* current_user_name){
    printf("\n------------------Phong choi %d ------------------\n", room->room_id);
    printf("- Trang thai: ");
    switch(room->status){
        case WAITING: printf("dang cho tran dau\n"); break;
        case PLAYING: printf("dang trong tran dau\n"); break;
    }
    printf("- So nguoi choi hien tai: %d\n", room->inroom_no);
    printf("- Nguoi choi:");
    printf("\n\t1. %s (chu phong)", room->players[0]);
    for(int j = 1; j < room->inroom_no; j++){
        printf("\n\t%d. %s", j+1, room->players[j]);
    }
    if(current_user_name != NULL){
        if(strcmp(current_user_name, room->players[0]) == 0){
            printf("\n1. Bat dau van dau");
            printf("\n2. Thoat phong\nLua chon cua ban: ");
            //printf("\nLua chon cua ban: ");
        } else {
            printf("\n2. Thoat phong\nLua chon cua ban: ");
            //printf("\nLua chon cua ban: ");
        }
        printf("");
        // printf("\nLua chon cua ban: ");
    }
}

char* roomToString(Room** root, int room_id){
    Room* room = root[room_id];
    char* str = (char*) malloc(100);
    snprintf(str, sizeof(str), "%d-%d", room->room_id, room->inroom_no);
    for(int i = 0; i < room->inroom_no; i++){
        strcat(str, "-");
        strcat(str, room->players[i]);
    }
    return str;
}