#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include "util.h"

#define SEND_RECV_LEN 100
#define MAX_ROOM_ALLOWED 4
#define MAX_PLAYER_PER_ROOM 4
#define MSG_NUM 10

//--------------------Globals--------------------

int current_no_room; //
int rooms[MAX_ROOM_ALLOWED][MAX_PLAYER_PER_ROOM];


//Remember to use -pthread when compiling this server's source code
void *connection_handler(void *);

// This function resolve message from client
void resolve(char* client_message, int socket);

void initGlobals(){
	current_no_room = 0;
	for(int i = 0; i < MAX_ROOM_ALLOWED; i++)
		for(int j = 0; j < MAX_PLAYER_PER_ROOM; j++)
			rooms[i][j] = -1;
}

//-------------------------- Main --------------------------

int main()
{
    char server_message[100] = "Hello from Server!!\n";
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (server_socket==-1){
		perror("Socket initialisation failed");
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
    
	// init globals
	initGlobals();

    int no_threads=0; // number of threads accepted
    pthread_t threads[3];
    while (no_threads<3){
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
		puts("Handler assigned");
		no_threads++;
	}
	int k=0;
    for (k=0;k<3;k++){
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
    send_status=send(socket, server_message, 100, 0);
	char client_message[100];
	char result_data[100];
	while( (read_len=recv(socket,client_message, 100,0))>0)
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
	FILE* readfp = fopen("accounts.txt", "r");
	int no_accounts;
	char username[50];
	char password[50];
	int active;
	printf("\nrecv: <%s><%s><%s>", msg[0], msg[1], msg[2]);
	fscanf(readfp, "%d\n", &no_accounts);
	printf("\n%d\n", no_accounts);
	for(int i = 1; i <= no_accounts; i++){
		fscanf(readfp, "%s %s %d\n", username, password, &active);
		printf("\n<%s><%s><%d>", username, password, active);
		if(strcmp(username, msg[1]) == 0 && strcmp(password, msg[2]) == 0){
			if(active == 1) {
				send(socket, "Tai khoan dang hoat dong.", 100, 0);
			}
			else {
				send(socket, "Dang nhap thanh cong", 100, 0);
				// chuyen active thanh 1, chua lam
			}
			fclose(readfp);
			return;
		}
	}
	send(socket, "Tai khoan khong ton tai", 100, 0);
	// strcpy(result, "Tai khoan khong hop le.");
	fclose(readfp);
}

void unknownMsg(int socket){
	send(socket, "Khong ro cau lenh", 100, 0);
}

void createRoom(char** msg, int sock){
	if(current_no_room == MAX_ROOM_ALLOWED){
		send(sock, "newroom#fail", SEND_RECV_LEN, 0);
		return;
	}
	current_no_room++;
	printf("\ncurrent_no_room = %d", current_no_room);
	send(sock, "newroom#success", SEND_RECV_LEN, 0);
}

void resolve(char* client_msg, int socket){
	char* melted_msg[MSG_NUM];
	int ele_count = meltMsg(client_msg, melted_msg);
	if(strcmp(melted_msg[0], "lgi") == 0){
		login(melted_msg, socket);
		return;
	}
	if(strcmp(melted_msg[0], "newroom") == 0){
		createRoom(melted_msg, socket);
		return;
	}
	if(strcmp(melted_msg[0], "logout") == 0){
		logout(melted_msg, socket);
		return;
	}
	else {
		unknownMsg(socket);
	}
}

