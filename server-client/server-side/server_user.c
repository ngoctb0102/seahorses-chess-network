#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../message.h"
#include "../../user/user.h"

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
    // TODO
}