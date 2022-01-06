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

void login(char** msg, int socket) {
	printf("\nrecv: <%s><%s><%s>", msg[0], msg[1], msg[2]);
	UserNode* node = searchUser(users, msg[1]);
	if(node == NULL){
		send(socket, "Tai khoan khong ton tai", 100, 0); // message
	}
	else {
		if(updateUserStatus(users, msg[1], ONLINE))
			send(socket, "Dang nhap thanh cong", SEND_RECV_LEN, 0); // message
		else{
			send(socket, "Tai khoan dang hoat dong.", SEND_RECV_LEN, 0); // message
		}
	}
}

void logout(char** msg, int sock){
	if(updateUserStatus(users, msg[1], OFFLINE))
		send(sock, "Dang xuat thanh cong", SEND_RECV_LEN, 0); // message
}

void signup(char** msg, int sock){
	// add to users tree
	UserNode* node = searchUser(users, msg[1]);
	if(node != NULL){
		send(sock, "SIGNUP-EXISTS", SEND_RECV_LEN, 0);
		return;
	}
	users = addUser(users, msg[1], msg[2]);
	updateUserStatus(users, msg[1], ONLINE);
	// write to users file
	FILE* fp = fopen("accounts.txt", "r+");
	if(fp == NULL){
		printf("Can't open users records");
		exit(1);
	}
	fprintf(fp, "%d", total_user+1);
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%s %s\n", msg[1], msg[2]);
	fclose(fp);
	send(sock, "SIGNUP-SUCCESS", SEND_RECV_LEN, 0);
}