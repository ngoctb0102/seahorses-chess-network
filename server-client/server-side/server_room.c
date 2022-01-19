#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../../room/room.h"
#include "../message.h"
#include "server_room.h"
#include "server_user.h"
#include "../../user/user.h"

void userCreateRoom(char** msg, UserNode** current_user){
	if(current_no_room == MAX_ROOM_ALLOWED){
		send((*current_user)->recv_sock, "newroom-full", SEND_RECV_LEN, 0);
		return;
	}

	current_no_room++;

	int room_id = addRoom(rooms, msg[1]);

	(*current_user)->status = INROOM;
	(*current_user)->room_id = room_id;

	printf("\ncurrent_no_room = %d", current_no_room);

	char buff[100];
	snprintf(buff, sizeof(buff), "NEWROOM-SUCCESS-%d", room_id);
	send((*current_user)->recv_sock, buff, SEND_RECV_LEN, 0);
}

void userExitRoom(char** msg, UserNode** current_user){
	char buff[LEN];
	Room* room = rooms[(*current_user)->room_id];
	// If exit one is room owner: remove all other players, close room and inform all players.
	if(strcmp((*current_user)->username, rooms[(*current_user)->room_id]->players[0]) == 0){
		for(int i = 0; i < room->inroom_no; i++){
			if(strcmp((*current_user)->username, room->players[i]) == 0)
				continue;
			else {
				UserNode* user = searchUser(users, room->players[i]);
				sprintf(buff, "UPDATEROOM-ALLEXIT-%s", (*current_user)->username); // message
				send(user->recv_sock, buff, sizeof(buff), 0);
			}
		}
		delRoom(rooms, (*current_user)->room_id);
		current_no_room -= 1;
	}
	// If exit one is not owner: update room and inform other players. 
	else {
		// First remove the exit user
		int i = 0;
		while(strcmp((*current_user)->username, room->players[i]) != 0) i++;
		while(i < room->inroom_no - 1){
			strcpy(room->players[i], room->players[i+1]);
			i++;
		}
		room->inroom_no -= 1;
		// Then inform players in rooms
		for(i = 0; i < room->inroom_no; i++){
			UserNode* user_in_room = searchUser(users, room->players[i]);
			sprintf(buff, "UPDATEROOM-EXIT-%s", (*current_user)->username); // message
			send(user_in_room->recv_sock, buff, sizeof(buff), 0);
		}
	}
}

void userJoinRoom(char** msg, UserNode** current_user){
	char buff[100];
	int room_id = atoi(msg[2]);
	// validate room id
	if(room_id < 0 || room_id >= MAX_ROOM_ALLOWED || rooms[room_id] == NULL){
		snprintf(buff, sizeof(buff), "JOINROOM-FAIL-%d", room_id); // message
		send((*current_user)->recv_sock, buff, sizeof(buff), 0);
		return;
	}

	Room* joinroom = rooms[room_id];
	if(joinroom->inroom_no >= MAX_PLAYER_PER_ROOM){
		snprintf(buff, sizeof(buff), "JOINROOM-FULL-%d", room_id);
		send((*current_user)->recv_sock, buff, SEND_RECV_LEN, 0);
		return;
	}

	// let current_user join the room and inform current_player
	strcpy(joinroom->players[joinroom->inroom_no], (*current_user)->username);
	joinroom->inroom_no += 1;
	(*current_user)->room_id = joinroom->room_id;
	(*current_user)->status = INROOM;
	UserNode* user = searchUser(users, (*current_user)->username);
	user->status = INROOM;
	snprintf(buff, sizeof(buff), "JOINROOM-SUCCESS-%s", roomToString(rooms, room_id));
	send((*current_user)->recv_sock, buff, sizeof(buff), 0);

	// inform all other players inrooms
	snprintf(buff, sizeof(buff), "UPDATEROOM-JOIN-%s", (*current_user)->username);
	for(int i = 0; i < joinroom->inroom_no-1; i++){
		UserNode* user = searchUser(users, joinroom->players[i]);
		send(user->recv_sock, buff, sizeof(buff), 0);
	}
}

