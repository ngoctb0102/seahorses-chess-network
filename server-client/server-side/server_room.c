#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../../room/room.h"
#include "../message.h"
#include "server_room.h"

void userCreateRoom(char** msg, int sock){
	if(current_no_room == MAX_ROOM_ALLOWED){
		send(sock, "newroom-full", SEND_RECV_LEN, 0);
		return;
	}
	current_no_room++;
	int room_id = addRoom(rooms, msg[1]);
	printf("\ncurrent_no_room = %d", current_no_room);
	char buff[100];
	snprintf(buff, sizeof(buff), "newroom-success-%d", room_id);
	send(sock, buff, SEND_RECV_LEN, 0);
}

void userExitRoom(char** msg, int sock){
	if(removeUserFromRoom(rooms, atoi(msg[2]), msg[1]) == 0)
		current_no_room--;
}

void userJoinRoom(char** msg, int sock){
    // TODO
	char buff[100];
	int room_id = atoi(msg[2]);
	if(room_id < 0 || room_id >= MAX_ROOM_ALLOWED || rooms[room_id] == NULL){
		snprintf(buff, sizeof(buff), "JOINROOM-FAIL-%d", room_id);
		send(sock, buff, sizeof(buff), 0);
		return;
	}
	switch(addUserToRoom(rooms, room_id, msg[1])){
		case 0:
			snprintf(buff, sizeof(buff), "JOINROOM-FAIL-%d", room_id); 
			break;
		case 1:
			snprintf(buff, sizeof(buff), "JOINROOM-SUCCESS-%s", roomToString(rooms, room_id));
			break;
		case -1:
			snprintf(buff, sizeof(buff), "JOINROOM-FULL-%d", room_id); 
			break;
	}
	send(sock, buff, SEND_RECV_LEN, 0);
}

