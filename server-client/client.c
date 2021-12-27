#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "util.h"
#include "../room/room.h"

#define MSG_LEN 100

//----------Globals--------------

char current_user_name[100];
Room* my_room = NULL;

//----------Functions------------

void initGlobals();
int login(int sock);
void logout(int sock);
void home(int sock);
void requestCreateRoom(int sock);
void requestJoinRoom();
void requestFindRoom();
void toRoomLobby(int sock);

//------------------------------

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
        // char msg[256];
        //receive data from the server
        recv(network_socket, response, MSG_LEN, 0);
        printf(">> %s\n", response);
        int choice;
        //char response[256];
        do{
            system("clear");
            printf("\n-------------GAME CA NGUA SIEU CAP VJPRO-------------");
            printf("\n1. Dang nhap");
            printf("\n2. Thoat");
            printf("\nLua chon cua ban: "); scanf("%d", &choice);
            switch(choice){
                case 1: 
                    if(login(network_socket)){
                        home(network_socket);
                    } break;
                case 2: 
                    send(network_socket, "exit", MSG_LEN, 0);
                    printf("\nHen gap lai!!\n"); 
                    break;
                default: printf("\nKhong hieu? Chon lai di.\n"); break;
            }
        } while(choice != 2);
	}
    //close the socket
    close(network_socket);
    
    return 0;
}

//------------------------------------------------------------

int login(int sock){
    char username[100];
    char password[100];
    char message[256];
    char response[256];
    while(1){
        system("clear");
        printf("\n----Dang nhap----");
        printf("\n> Ten nguoi dung (go 'quit' de thoat): "); scanf("%s", username);
        if(strcmp(username, "quit") == 0)
            return 0;
        printf("\n> Mat khau: "); scanf("%s", password);
        strcpy(message, "lgi-");
        strcat(message, username);
        strcat(message, "-");
        strcat(message, password);
        send(sock, message, MSG_LEN , 0);
        recv(sock, response, MSG_LEN, 0);
        printf("\n%s", response);
        if(strcmp(response, "Dang nhap thanh cong") == 0){
            strcpy(current_user_name, username);
            return 1;
        }
    }
}

void logout(int sock){
    char buff[100];
    strcpy(buff, "logout-");
    strcat(buff, current_user_name);
    send(sock, buff, MSG_LEN, 0);
    recv(sock, buff, MSG_LEN, 0);
    strcpy(current_user_name, "");
}

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
            case 1: requestCreateRoom(sock); break;
            case 2: requestJoinRoom(); break;
            case 3: requestFindRoom(); break;
            case 4: logout(sock); break;
            default: printf("\nLa sao? Nhap lai coi\n"); break;
        }
    } while(choice != 4);
}

void requestCreateRoom(int sock){
    // TODO
    char buff[100];
    char res[100];
    char* melted_msg[10];
    strcpy(buff, "newroom");
    strcat(buff, "-");
    strcat(buff, current_user_name);
    send(sock, buff, MSG_LEN, 0);
    recv(sock, res, MSG_LEN, 0);
    meltMsg(res, melted_msg);
    if(strcmp(melted_msg[0], "newroom") == 0){
        if(strcmp(melted_msg[1], "success") == 0){
            printf("\n >> From server: Tao phong thanh cong\n");
            my_room = createRoom(atoi(melted_msg[2]), current_user_name);
            toRoomLobby(sock);
        }
        else if(strcmp(melted_msg[1], "full") == 0)
            printf("\n >> From server: So phong da dat toi da.\n");
        else printf("\n >> From server: Tao phong khong thanh cong\n");
    }
}

void requestJoinRoom(){
    // TODO
}

void requestFindRoom(){
    // TODO
}

void exitRoom(int sock){
    char buff[100];
    snprintf(buff, sizeof(buff), "exitroom-%s-%d", current_user_name, my_room->room_id);
    send(sock, buff, MSG_LEN, 0);
    Room* node = my_room;
    freeRoom(node);
    my_room = NULL;
}

void toRoomLobby(int sock){
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