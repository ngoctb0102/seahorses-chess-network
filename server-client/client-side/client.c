#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../util.h"
#include "../../room/room.h"
#include "client_user.h"
#include "client_home.h"
#include "../message.h"

//-------------Globals-----------------

char current_user_name[100];
Room* my_room = NULL;

//----------User Interfaces------------

void home(int sock);
void roomLobby(int sock);

//------------------------------------

int main(int argc, const char * argv[]) {
    //create a socket
    int network_socket;
    network_socket=socket(AF_INET,SOCK_STREAM,0);
    
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9999);
    server_address.sin_addr.s_addr=INADDR_ANY;
    
    int connection_status = connect(network_socket, (struct sockaddr *) & server_address, sizeof(server_address));
    //check for connection_status
    if(connection_status==-1)
    {
        printf("The connection has error\n\n");
    }
    
    if(connection_status==0)
    {
        char response[256];
        //receive data from the server
        recv(network_socket, response, SEND_RECV_LEN, 0);
        printf(">> %s\n", response);
        int choice;
        do{
            system("clear");
            printf("\n-------------GAME CA NGUA SIEU CAP VJPRO-------------");
            printf("\n1. Dang nhap");
            printf("\n2. Dang ki");
            printf("\n3. Thoat");
            printf("\nLua chon cua ban: "); scanf("%d", &choice);
            switch(choice){
                case 1: 
                    if(login(network_socket)){
                        home(network_socket);
                    } break;
                case 2:
                    if(signup(network_socket)){
                        home(network_socket);
                    }
                    break;
                case 3: 
                    send(network_socket, "exit", SEND_RECV_LEN, 0);
                    printf("\nHen gap lai!!\n"); 
                    break;
                default: printf("\nKhong hieu? Chon lai di.\n"); break;
            }
        } while(choice != 3);
	}
    //close the socket
    close(network_socket);
    
    return 0;
}

//------------------------------------------------------------

void home(int sock){
    int choice;
    do {
        system("clear");
        printf("\n-----------Sanh cho-----------");
        printf("\nXin chao %s", current_user_name);
        printf("\n1. Tao phong");
        printf("\n2. Tham gia phong");
        printf("\n3. Tim phong choi");
        printf("\n4. Thoat");
        printf("\nLua chon cua ban: "); scanf("%d", &choice);
        switch(choice){
            case 1: 
                requestCreateRoom(sock);
                if(my_room != NULL)
                    roomLobby(sock);
                break;
            case 2: 
                requestJoinRoom(sock); 
                if(my_room != NULL)
                    roomLobby(sock);
                break;
            case 3: requestFindRoom(sock); break;
            case 4: logout(sock); break;
            default: printf("\nLa sao? Nhap lai coi\n"); break;
        }
    } while(choice != 4);
}

void roomLobby(int sock){
    int choice;
    do{
        system("clear");
        printRoom(my_room);
        printf("\n1. Bat dau van dau");
        printf("\n2. Thoat");
        printf("\nLua chon cua ban: "); scanf("%d", &choice);
        switch(choice){
            case 1: break;
            case 2: exitRoom(sock); break;
            default: printf("\nKhong ro cau lenh.\n"); break;
        }
    } while(choice != 2);
}