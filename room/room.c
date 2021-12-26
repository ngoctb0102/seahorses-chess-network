#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "room.h"

RoomNode* createRoom(int room_id, char* owner){
    RoomNode* newroom = (RoomNode*) malloc(sizeof(RoomNode));
    newroom->game = NULL;
    newroom->room_id = room_id;
    newroom->inroom_no = 1;
    newroom->left = NULL;
    newroom->right = NULL;
    newroom->status = WAITING;
    for(int i = 0; i < MAX_PLAYER_PER_ROOM; i++){
        newroom->players[i] = (char*) malloc(100);
    }
    strcpy(newroom->players[0], owner);
    return newroom;
}

RoomNode* addRoom(RoomNode* root, int room_id, char* owner){
    if(root == NULL) root = createRoom(room_id, owner);
    else {
        RoomNode* node = searchRoom(root, room_id);
        if(node == NULL){
            if(root->room_id < room_id) root->right = addRoom(root->right , room_id, owner);
            if(root->room_id > room_id) root->left = addRoom(root->left , room_id, owner);
        }
    }
    return root;
}

RoomNode* searchRoom(RoomNode* root, int room_id){
    if(root == NULL) return NULL;
    else {
        if(root->room_id < room_id)
            root = searchRoom(root->right, room_id);
        else {
            if(root->room_id > room_id)
                root = searchRoom(root->left, room_id);
        }
    }
    return root;
}

int addUserToRoom(RoomNode* root, int room_id, char* username){
    if(root == NULL) return -1;
    RoomNode* node = searchRoom(root, room_id);
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

void removeUserFromRoom(RoomNode** root, int room_id, char* username){
    // TODO
    RoomNode* node = searchRoom(*root, room_id);
    if(node == NULL) return;
    if(strcmp(node->players[0], username) == 0)
        delRoom(root, room_id);
    else{
        int i = 1;
        while(strcmp(node->players[i], username) != 0) i++;
        while(i < node->inroom_no-1) {
            strcpy(node->players[i], node->players[i+1]);
            i++;
        }
        node->inroom_no -= 1;
    }
}

void freeRoom(RoomNode* node){
    for(int i = 0; i < MAX_PLAYER_PER_ROOM; i++){
        free(node->players[i]);
    }
    free(node);
}

void delRoom(RoomNode** root, int room_id){
    // TODO
    RoomNode* node = searchRoom(*root, room_id);
    if(node == NULL) return;

    // case 1: node is leaf
    if(node->left == NULL && node->right == NULL){
        freeRoom(node);
        node = NULL;
    }
    // case 2: node not leaf
    else if(node->right != NULL){
        if(node->room_id == (*root)->room_id){
            *root = (*root)->right;
        } else {
            RoomNode* node1 = *root;
            while(node1->right->room_id != node->room_id)
                node1 = node1->right;
            node1->right = node->right;
        }
        freeRoom(node);
    }
}

void startGame(RoomNode* root, int room_id){
    // TODO
    RoomNode* node = searchRoom(root, room_id);
    if(node == NULL) return;
    node->status = PLAYING;
}

void printRoomTree(RoomNode* root){
    if(root == NULL) return;
    printRoomTree(root->right);
    printRoom(root);
    printRoomTree(root->left);
    printf("\n");
}

void printRoom(RoomNode* room){
    printf("\n------------------Phong choi %d ------------------\n", room->room_id);
    printf("- Trang thai: ");
    switch(room->status){
        case WAITING: printf("dang cho tran dau\n"); break;
        case PLAYING: printf("dang trong tran dau\n"); break;
    }
    printf("- So nguoi choi hien tai: %d\n>", room->inroom_no);
    printf("- Nguoi choi:\n");
    printf("\n\t1. %s (chu phong)", room->players[0]);
    for(int i = 1; i < room->inroom_no; i++){
        printf("\n\t%d. %s", i+1, room->players[i]);
    }
}