#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../message.h"
#include "client_user.h"
#include "client_home.h"
#include "client.h"
#include "../../room/room.h"
#include "../util.h"

Room* createJoinRoom(char** msg){
    int room_id = atoi(msg[2]);
    int inroom_no = atoi(msg[3]);
    Room* joinroom = (Room*) malloc(sizeof(Room));
    joinroom->game = NULL;
    joinroom->inroom_no = inroom_no;
    joinroom->room_id = room_id;
    joinroom->status = WAITING;
    int i = 0;
    for(; i < inroom_no; i++){
        joinroom->players[i] = (char*) malloc(100);
        strcpy(joinroom->players[i], msg[i+4]);
    }
    for(; i < MAX_PLAYER_PER_ROOM; i++){
        joinroom->players[i] = (char*) malloc(100);
    }
    return joinroom;
}

void requestCreateRoom(int sock){
    char buff[100];
    sprintf(buff, "NEWROOM-%s", current_user->username);
    send(sock, buff, SEND_RECV_LEN, 0);
    state = WAITING_RESPONSE;
}

int requestJoinRoom(int sock){
    char buff[BUFFSIZE];
    while(1){
        while(state == WAITING_RESPONSE);
        send(sock, "ROOMS", SEND_RECV_LEN, 0);
        state = WAITING_RESPONSE;
        while(state == WAITING_RESPONSE);
        int join_id = -99;
        printf("\n> Moi nhap ma so phong tham gia (nhap '-1' de thoat): ");
        scanf("%d%*c", &join_id);
        if(join_id == -1) {
            return 0;
        }
        else {
            sprintf(buff, "JOINROOM-%s-%d", current_user->username, join_id);
            send(current_user->send_sock, buff, SEND_RECV_LEN, 0);
            state = WAITING_RESPONSE;
            while(state == WAITING_RESPONSE);
            if(state == IN_ROOM) break;
        }
    }
    return 1;
}

void exitRoom(int sock){
    char buff[100];
    snprintf(buff, sizeof(buff), "exitroom-%s-%d", current_user->username, my_room->room_id); // message
    send(sock, buff, SEND_RECV_LEN, 0);
    Room* node = my_room;
    freeRoom(node);
    state = LOGGED_IN;
    my_room = NULL;
}