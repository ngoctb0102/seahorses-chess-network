#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "../util.h"
#include "../../room/room.h"
#include "client_user.h"
#include "client_home.h"
#include "client_game.h"
#include "../message.h"
#include "../../user/user.h"
#include "client.h"

//-------------Globals-----------------

UserNode* current_user = NULL;
int state = NOT_LOGGED_IN;
int room_updating = 0;
Room* my_room = NULL;

//----------User Interfaces------------

void home(int sock);
void roomLobby(int sock);

//------------ Handlers --------------

void* recv_handler(void* recv_sock);
void* send_handler(void* send_sock);

//------------------------------------

int main(int argc, const char * argv[]) {
    // create sockets
    int client_send_sock;
    client_send_sock = socket(AF_INET, SOCK_STREAM, 0);

    int client_recv_sock;
    client_recv_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9999);
    server_address.sin_addr.s_addr=INADDR_ANY;

    if(connect(client_send_sock, (struct sockaddr *) &server_address, sizeof(server_address)) == -1){
        puts("Unable to connect send-stream to server. Exit");
        exit(-1);
    }

    if(connect(client_recv_sock, (struct sockaddr *) &server_address, sizeof(server_address)) == -1){
        puts("Unable to connect recv-stream to server. Exit");
        exit(-1);
    }

    // Threading
    pthread_t threads[2];

    if(pthread_create(&threads[0], NULL, send_handler, &client_send_sock) < 0){
        puts("Unable to open send thread. Exit.");
        exit(-1);
    }

    if(pthread_create(&threads[1], NULL, recv_handler, &client_recv_sock) < 0){
        puts("Unable to open recv thread. Exit");
        exit(-1);
    }

    // join threads
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // close sockets
    close(client_send_sock);
    close(client_recv_sock);
    
    return 0;
}

//------------------------------------------------------------

void home(int sock){
    int choice;
    do {
        system("clear");
        printf("\n-----------Sanh cho-----------");
        printf("\nXin chao %s", current_user->username);
        printf("\n1. Tao phong");
        printf("\n2. Tham gia phong");
        printf("\n3. Tim phong choi");
        printf("\n4. Thoat");
        printf("\n5. send something");
        printf("\nLua chon cua ban: "); scanf("%d%*c", &choice);
        switch(choice){
            case 1: 
                requestCreateRoom(sock);
                // if(my_room != NULL)
                roomLobby(sock);
                break;
            case 2: 
                requestJoinRoom(sock); 
                // if(my_room != NULL)
                roomLobby(sock);
                break;
            case 3: requestFindRoom(sock); break;
            case 4: request_logout(sock); break;
            case 5: send(sock, "TO-tuanvu-hi i am long", SEND_RECV_LEN, 0); break;
            default: printf("\nLa sao? Nhap lai coi\n"); break;
        }
    } while(choice != 4);
}

void roomLobby(int sock){
    int choice;
    while(state == IN_ROOM || state == WAITING_RESPONSE){
        if(state != WAITING_RESPONSE){
        // if(state == IN_ROOM && room_updating == 0){
            scanf("%d%*c", &choice);
            switch(choice){
                case 1: startGame(sock); break;
                case 2: exitRoom(sock); break;
                default: printf("\nKhong ro cau lenh.\n"); break;
            }
            if(choice == 2) break;
        }
    }
}

//------------------------------------------------------------

void* send_handler(void* send_sock){
    int send_socket = *(int*) send_sock;
    int choice;
    while(state == WAITING_RESPONSE || state == NOT_LOGGED_IN) {
        //system("clear");
        if(state == NOT_LOGGED_IN){
            printf("\n-------------GAME CA NGUA SIEU CAP VJPRO-------------");
            printf("\n1. Dang nhap");
            printf("\n2. Dang ki");
            printf("\n3. Thoat");
            printf("\nLua chon cua ban: "); scanf("%d", &choice);
            switch(choice){
                case 1:
                    if(request_login(send_socket)){
                        home(send_socket);
                    }
                    break;
                case 2:
                    if(request_signup(send_socket)){
                        home(send_socket);
                    }
                    break;
                case 3: 
                    send(send_socket, "exit", SEND_RECV_LEN, 0);
                    printf("\nHen gap lai!!\n"); 
                    break;
                default: printf("\nKhong hieu? Chon lai di.\n"); break;
            }
            if(choice == 3) break;
        }
    }

    return 0;
}

void* recv_handler(void* recv_sock){
    int recv_socket = *(int*) recv_sock;
    int recv_bytes;
    char buff[LEN];
    char* msg[10];
    while((recv_bytes = recv(recv_socket, buff, SEND_RECV_LEN, 0) > 0)){
        meltMsg(buff, msg);
        if(strcmp(msg[0], "LOGIN") == 0){
            if(strcmp(msg[1], "SUCCESS") == 0){
                puts("\nlogin sucess");
                current_user = createUserNode(msg[2], msg[3]);
                state = LOGGED_IN;
                continue;
            }
            if(strcmp(msg[1], "FAILED") == 0){
                puts("\nlogin failed.");
                state = NOT_LOGGED_IN;
                continue;
            }
        }
        if(strcmp(msg[0], "LOGOUT") == 0){
            if(strcmp(msg[1], "SUCCESS") == 0){
                state = NOT_LOGGED_IN;
                UserNode* node = current_user;
                free(node);
                current_user = NULL;
                continue;
            } else {
                state = LOGGED_IN;
                continue;
            }
        }
        if(strcmp(msg[0], "SIGNUP") == 0){
            if(strcmp(msg[1], "SUCCESS") == 0){
                current_user = createUserNode(msg[2], msg[3]);
                state = LOGGED_IN;
                continue;
            } else {
                state = NOT_LOGGED_IN;
                continue;
            }
        }
        if(strcmp(msg[0], "FROM") == 0){ // experiment
            printf("\nFrom %s: %s\n", msg[1], msg[2]);
            continue;
        }
        if(strcmp(msg[0], "NEWROOM") == 0){
            if(strcmp(msg[1], "SUCCESS") == 0){
                my_room = createRoom(atoi(msg[2]), current_user->username);
                room_updating = 1;
                printRoom(my_room, current_user->username);
                room_updating = 0;
                state = IN_ROOM;
                continue;
            }
        }
        if(strcmp(msg[0], "UPDATEROOM") == 0){
            if(strcmp(msg[1], "JOIN") == 0){
                system("clear");
                strcpy(my_room->players[my_room->inroom_no], msg[2]);
                my_room->inroom_no += 1;
                room_updating = 1;
                printf("%s joined", msg[2]);                
                printRoom(my_room, current_user->username);
                room_updating = 0;
                continue;
            }
        }
        if(strcmp(msg[0], "JOINROOM") == 0){ // message
            if(strcmp(msg[1], "SUCCESS") == 0){ // message
                system("clear");
                my_room = createJoinRoom(msg);
                room_updating = 1;
                printf("\n>> From server: Tham gia phong thanh cong\n");
                printRoom(my_room, current_user->username);
                room_updating = 0;
                state = INROOM;
                continue;
            }
            else if(strcmp(msg[1], "FULL") == 0) {// message
                printf("\n >> From server: So nguoi choi trong phong da dat toi da.\n");
                continue;
            }
            else { 
                printf("\n >> From server: Tham gia phong khong thanh cong\n");
                continue;
            }
        }
    }

    if(recv_bytes < 0){
        puts("\nError occurs in connection");
    } else {
        puts("\nConnection closed");
    }

    return 0;
}