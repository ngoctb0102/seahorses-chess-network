#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../message.h"
#include "client_user.h"
#include "client_home.h"
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
    // TODO
    char buff[100];
    char res[100];
    char* melted_msg[10];
    strcpy(buff, "newroom");            // message
    strcat(buff, "-");                  //
    strcat(buff, current_user_name);    //
    send(sock, buff, SEND_RECV_LEN, 0);
    recv(sock, res, SEND_RECV_LEN, 0);
    meltMsg(res, melted_msg);
    if(strcmp(melted_msg[0], "newroom") == 0){ // message
        if(strcmp(melted_msg[1], "success") == 0){ // message
            printf("\n >> From server: Tao phong thanh cong\n");
            my_room = createRoom(atoi(melted_msg[2]), current_user_name);
        }
        else if(strcmp(melted_msg[1], "full") == 0) // message
            printf("\n >> From server: So phong da dat toi da.\n");
        else printf("\n >> From server: Tao phong khong thanh cong\n");
    }
}

void requestJoinRoom(int sock){
    char buff[100];
    char res[100];
    char* melted_msg[10];
    int room_id;
    printf("> Nhap ma so phong ban muon tham gia: "); scanf("%d", &room_id);
    snprintf(buff, sizeof(buff), "JOINROOM-%s-%d", current_user_name, room_id); // message
    send(sock, buff, SEND_RECV_LEN, 0);
    recv(sock, res, SEND_RECV_LEN, 0);
    meltMsg(res, melted_msg);
    if(strcmp(melted_msg[0], "JOINROOM") == 0){ // message
        if(strcmp(melted_msg[1], "SUCCESS") == 0){ // message
            printf("\n>> From server: Tham gia phong thanh cong\n");
            my_room = createJoinRoom(melted_msg);
        }
        else if(strcmp(melted_msg[1], "FULL") == 0) // message
            printf("\n >> From server: So nguoi choi trong phong da dat toi da.\n");
        else printf("\n >> From server: Tham gia phong khong thanh cong\n");
    }
}

void requestFindRoom(){
    char buff[100];
    snprintf(buff, sizeof(buff), "FINDROOM");
}

void exitRoom(int sock){
    char buff[100];
    snprintf(buff, sizeof(buff), "exitroom-%s-%d", current_user_name, my_room->room_id); // message
    send(sock, buff, SEND_RECV_LEN, 0);
    Room* node = my_room;
    freeRoom(node);
    my_room = NULL;
}