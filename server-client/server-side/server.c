#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include "../util.h"
#include "../../user/user.h"
#include "../../room/room.h"
#include "../../gameplay/game.h"
#include "server_room.h"
#include "server_user.h"
#include "../message.h"
#include "server.h"

#define MSG_NUM 10

//------------------Globals----------------------

int current_no_room; // current number of room on server
int total_user;
int server_socket;
UserNode* users;
Room* rooms[MAX_ROOM_ALLOWED];

//---------------------Base functions declarations---------------------

//Remember to use -pthread when compiling this server's source code
void *connection_handler(void *);
void resolve(char* client_message, UserNode* current_user, int client_send_sock, int client_recv_sock);
void initGlobals();


//-------------------------- Main --------------------------

int main()
{
	// init globals
	initGlobals();

	// ------------- setup server socket -------------
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (server_socket==-1){
		perror("Socket initialization failed");
		exit(EXIT_FAILURE);
	} else printf("Server socket created successfully\n");

	int option = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
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

	// --------- Handling connection from clients -----------

	struct sockaddr_in client_addr;
	int client_send_sock;
	int client_recv_sock;
	int sin_size = sizeof((struct sockaddr *)&client_addr);

    int no_threads=0; // number of threads accepted
    pthread_t threads[MAX_CLIENT_ALLOWED];

    while (no_threads < MAX_CLIENT_ALLOWED){
		printf("Listening...\n");
		client_send_sock = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);
		if(client_send_sock < 0){
			printf("Server failed to accept client's send-stream\n");
			exit(0);
		} else puts("> Client's send-sock accepted");

		client_recv_sock = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);
		if(client_recv_sock < 0){
			printf("Server failed to accept client's recv-stream\n");
			exit(0);
		} else puts("> Client's recv-sock accepted");
		puts("Connection accepted");

		ThrdHandlerArgs args;
		args.client_recv_sock = client_recv_sock;
		args.client_send_sock = client_send_sock;
		if(pthread_create(&threads[no_threads], NULL, connection_handler, &args) < 0){
			perror("Could not create thread");
			return 1;
		}

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

//---------------------------Base functions definitions---------------------------

void initGlobals(){
	printf("\n=====================================");
	printf("\nSetting up globals.....");
	current_no_room = 0;
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

void *connection_handler(void *client_sockets){
	// int socket = *(int*) client_socket;
	int client_send_sock = (*(ThrdHandlerArgs *) client_sockets).client_send_sock;
	int client_recv_sock = (*(ThrdHandlerArgs *) client_sockets).client_recv_sock;
	
	char* msg[MSG_NUM];
	
	UserNode* current_user = NULL;

	int read_len;

	char client_message[100];
	while((read_len = recv(client_send_sock, client_message, SEND_RECV_LEN,0)) > 0)
	{
		client_message[read_len] = '\0';
		printf("\n> Recv: %s", client_message);
		if(strcmp(client_message, "exit") == 0){
			break;
		}

		meltMsg(client_message, msg);
		if(strcmp(msg[0], "lgi") == 0){ // message prefix
			current_user = login(msg, client_send_sock, client_recv_sock);
			continue;
		}
		if(strcmp(msg[0], "SIGNUP") == 0){
			signup(msg, &current_user, client_send_sock, client_recv_sock);
			continue;
		}
		if(strcmp(msg[0], "logout") == 0){ // message prefix
			logout(msg, &current_user);
			continue;
		}
		if(strcmp(msg[0], "newroom") == 0){ // message prefix
			userCreateRoom(msg, &current_user);
			continue;
		}
		if(strcmp(msg[0], "exitroom") == 0){ // message prefix
			userExitRoom(msg, &current_user);
			continue;
		}
		if(strcmp(msg[0], "JOINROOM") == 0){
			userJoinRoom(msg, &current_user);
			continue;
		}
		if(strcmp(msg[0], "TO") == 0){ // experiment
			UserNode* target_user = searchUser(users, msg[1]);
			if(target_user == NULL) {
				printf("Non existed target"); 
				continue;
			}
			if(target_user->status == OFFLINE) {
				printf("target user is offline"); 
				continue;
			}
			char buff[LEN];
			sprintf(buff, "FROM-%s-%s", current_user->username, msg[2]);
			send(target_user->recv_sock, buff, SEND_RECV_LEN, 0);
			continue; // experiment
		}
		if(strcmp(msg[0], "STARTC") == 0){
			Room* room = rooms[current_user->room_id];
			room->game = makeGame(room->room_id, room->inroom_no, room->players[0], room->players[1], room->players[2], room->players[3]);
			printGame(room->game);
		}
		else {
			send(client_recv_sock, "UNKNOWN", SEND_RECV_LEN, 0); // message
		}
	}

	close(client_send_sock);
	close(client_recv_sock);

	return 0;
}