#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../message.h"
#include "../../user/user.h"
#include "server_user.h"

UserNode* users;

UserNode* login(char** msg, int client_send_sock, int client_recv_sock) {
	char buff[BUFFSIZE];
	UserNode* node = searchUser(users, msg[1]);
	if(node == NULL){
		send(client_recv_sock, "LOGIN-FAILED-NONEXIST", SEND_RECV_LEN, 0); // message
	}
	else {
		if(node->status != OFFLINE)
			send(client_recv_sock, "LOGIN-FAILED-ACTIVE", SEND_RECV_LEN, 0); // message
		else{
			node->status = ONLINE;
			node->recv_sock = client_recv_sock;
			node->send_sock = client_send_sock;
			printf("\nUser logged in: %s\n", node->username);
			sprintf(buff, "LOGIN-SUCCESS-%s-%s", msg[1], msg[2]);
			send(client_recv_sock, buff, SEND_RECV_LEN, 0); // message
			return node;
		}
	}
}

void logout(char** msg, UserNode** current_user){
	if(updateUserStatus(users, msg[1], OFFLINE)){
		send((*current_user)->recv_sock, "LOGOUT-SUCCESS", SEND_RECV_LEN, 0); // message
		*current_user = NULL;
	}
}

void signup(char** msg, UserNode** current_user, int client_send_sock, int client_recv_sock){
	// add to users tree
	UserNode* node = searchUser(users, msg[1]);
	if(node != NULL){
		send((*current_user)->recv_sock, "SIGNUP-EXISTS", SEND_RECV_LEN, 0);
		return;
	}
	users = addUser(users, msg[1], msg[2]);
	updateUserStatus(users, msg[1], ONLINE);

	// make to current_user
	*current_user = searchUser(users, msg[1]);
	(*current_user)->recv_sock = client_recv_sock;
	(*current_user)->send_sock = client_send_sock;

	// write new account to users file
	FILE* fp = fopen("accounts.txt", "r+");
	if(fp == NULL){
		printf("Can't open users records");
		send((*current_user)->recv_sock, "SIGNUP-FAIL", SEND_RECV_LEN, 0);
		return;
	}
	fprintf(fp, "%d", total_user+1);
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%s %s\n", msg[1], msg[2]);
	fclose(fp);
	
	printf("\nUser signed up: %s\n", (*current_user)->username);
	// send sign up ACK
	char buff[LEN];
	sprintf(buff, "SIGNUP-SUCCESS-%s-%s", (*current_user)->username, (*current_user)->password);
	send((*current_user)->recv_sock, buff, SEND_RECV_LEN, 0);
}