#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include "util.h"
#include "../user/user.h"
#include "../room/room.h"

#define MAX_CLIENT_ALLOWED 20
#define SEND_RECV_LEN 100
#define MSG_NUM 10

//------------------Globals----------------------

int current_no_room; // current number of room on server
int total_user;
int current_no_user;
int server_socket;
UserNode* users;
Room* rooms[MAX_ROOM_ALLOWED];

//-----------------Functions---------------------

//Remember to use -pthread when compiling this server's source code
void *connection_handler(void *);
void resolve(char* client_message, int socket);

void initGlobals(){
	printf("\n=====================================");
	printf("\nSetting up globals.....");
	current_no_room = 0;
	current_no_user = 0;
	for(int i = 0; i < MAX_ROOM_ALLOWED; i++){
		rooms[i] = NULL;
	}
	FILE* fp = fopen("accounts.txt", "r");
	if(fp == NULL){
		printf("\n[ERROR] Unable to open accounts db");
		exit(1);
	}
    char username[100];
    char passwd[100];
    fscanf(fp, "%d\n", &total_user);
    for(int i = 1; i <= total_user; i++){
        fscanf(fp, "%s %s\n", username, passwd);
        users = addUser(users, username, passwd);
    }
	fclose(fp);
	printf("\nDone setup globals");
	printf("\n=====================================\n");
}

//-------------------------- Main --------------------------

int main()
{
	// init globals
	initGlobals();

    char server_message[100] = "Hello from Server!!\n";
    // int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (server_socket==-1){
		perror("Socket initialization failed");
		exit(EXIT_FAILURE);
	} else printf("Server socket created successfully\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    //bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr))!=0){
		printf("socket bind failed...\n"); 
        exit(0);
	}
    else printf("Socket successfully binded..\n"); 
    
    if (listen(server_socket, 3)!=0){
		printf("Listen failed...\n"); 
        exit(0); 
    } 
    else printf("Server listening..\n");

    int no_threads=0; // number of threads accepted
    pthread_t threads[MAX_CLIENT_ALLOWED];
    while (no_threads < MAX_CLIENT_ALLOWED){
		printf("Listening...\n");
		int client_socket = accept(server_socket, NULL, NULL);
		puts("Connection accepted");

		// create thread and assign handler here
		if( pthread_create( &threads[no_threads], NULL ,  connection_handler , &client_socket) < 0){
			perror("Could not create thread");
			return 1;
		}
		if (client_socket < 0) { 
			printf("server acccept failed...\n"); 
			exit(0); 
		} 
		else printf("Server acccept the client...\n");
		puts("Handler assigned\n");
		no_threads++;
	}
	int k=0;
    for (k=0; k < MAX_CLIENT_ALLOWED; k++){
		pthread_join(threads[k],NULL);
	}

    //int send_status;
    //send_status=send(client_socket, server_message, sizeof(server_message), 0);
    close(server_socket);
    
    return 0;
}


void *connection_handler(void *client_socket){
	int socket = *(int*) client_socket;
	int read_len;
	char server_message[100]="Hello from server\n";
	int send_status;
    send_status=send(socket, server_message, SEND_RECV_LEN, 0);
	char client_message[100];
	char result_data[100];
	while( (read_len=recv(socket,client_message, SEND_RECV_LEN,0))>0)
	{
		//end of string marker
		client_message[read_len] = '\0';
		if(strcmp(client_message,"exit")==0){break;}
		resolve(client_message, socket);
		printf("\nResult: %s[readlen=%d][client-msg-len=%d]", result_data, read_len, strlen(client_message));
		//Send the message back to client
		// send_status=send(socket , result_data , 100 ,0);	
	}
	return 0;
}

//----------------------------------------------------------------------

void login(char** msg, int socket) {
	printf("\nrecv: <%s><%s><%s>", msg[0], msg[1], msg[2]);
	UserNode* node = searchUser(users, msg[1]);
	if(node == NULL){
		send(socket, "Tai khoan khong ton tai", 100, 0);
	}
	else {
		if(updateUserStatus(users, msg[1], ONLINE))
			send(socket, "Dang nhap thanh cong", SEND_RECV_LEN, 0);
		else{
			send(socket, "Tai khoan dang hoat dong.", SEND_RECV_LEN, 0);
		}
	}
}

void logout(char** msg, int sock){
	// TODO
	if(updateUserStatus(users, msg[1], OFFLINE))
		send(sock, "Dang xuat thanh cong", SEND_RECV_LEN, 0);
}

void unknownMsg(int socket){
	send(socket, "Khong ro cau lenh", SEND_RECV_LEN, 0);
}

void createNewRoom(char** msg, int sock){
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
	removeUserFromRoom(rooms, atoi(msg[2]), msg[1]);
	current_no_room--;
}

void resolve(char* client_msg, int socket){
	char* melted_msg[MSG_NUM];
	int ele_count = meltMsg(client_msg, melted_msg);
	if(strcmp(melted_msg[0], "lgi") == 0){
		login(melted_msg, socket);
		return;
	}
	if(strcmp(melted_msg[0], "newroom") == 0){
		createNewRoom(melted_msg, socket);
		return;
	}
	if(strcmp(melted_msg[0], "logout") == 0){
		logout(melted_msg, socket);
		return;
	}
	if(strcmp(melted_msg[0], "exitroom") == 0){
		userExitRoom(melted_msg, socket);
	}
	else {
		unknownMsg(socket);
	}
}

