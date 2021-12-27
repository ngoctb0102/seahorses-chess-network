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

void requestJoinRoom(){
    // TODO
}

void requestFindRoom(){
    // TODO
}

void exitRoom(int sock){
    char buff[100];
    snprintf(buff, sizeof(buff), "exitroom-%s-%d", current_user_name, my_room->room_id); // message
    send(sock, buff, SEND_RECV_LEN, 0);
    Room* node = my_room;
    freeRoom(node);
    my_room = NULL;
}