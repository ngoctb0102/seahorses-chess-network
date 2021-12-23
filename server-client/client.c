#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define MSG_LEN 100

//----------Functions------------

int login(int sock);
void home(int sock);
void requestCreateRoom();
void requestJoinRoom();
void requestFindRoom();

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

        // int send_status= send(network_socket,msg, MSG_LEN ,0);
        // if (strcmp(msg,"exit")==0){
        //     break;
        // }
        // //print out the server's response
        // recv(network_socket, response, 100, 0);
        // printf("Here is the echo message from the server: %s\n\n", response);
        // // flush response
        // strcpy(response, "");
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
        printf("\n----Dang nhap----");
        printf("\n> Ten nguoi dung (go 'quit' de thoat): "); scanf("%s", username);
        if(strcmp(username, "quit") == 0)
            return 0;
        printf("\n> Mat khau: "); scanf("%s", password);
        strcpy(message, "lgi#");
        strcat(message, username);
        strcat(message, "#");
        strcat(message, password);
        send(sock, message, MSG_LEN , 0);
        recv(sock, response, MSG_LEN, 0);
        printf("\n%s", response);
        if(strcmp(response, "Dang nhap thanh cong") == 0)
            return 1;
    }
}

void home(int sock){
    int choice;
    do {
        printf("\n----Sanh cho----");
        printf("\n1. Tao phong");
        printf("\n2. Tham gia phong");
        printf("\n3. Tim phong choi");
        printf("\n4. Thoat");
        printf("\nLua chon cua ban: "); scanf("%d", &choice);
        switch(choice){
            case 1: requestCreateRoom(); break;
            case 2: requestJoinRoom(); break;
            case 3: requestFindRoom(); break;
            case 4: break;
            default: printf("\nLa sao? Nhap lai coi\n"); break;
        }
    } while(choice != 4);
}

void requestCreateRoom(){
    // TODO
}

void requestJoinRoom(){
    // TODO
}

void requestFindRoom(){
    // TODO
}