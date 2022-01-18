#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "../message.h"
#include "client_user.h"
#include "client.h"

extern UserNode* current_user;

// -------------- Request -------------------
int requestLogin(int sock){
    char username[100];
    char password[100];
    char message[256];
    // char response[256];
    while(state == NOT_LOGGED_IN || state == WAITING_RESPONSE){
        if(state == NOT_LOGGED_IN){
            printf("\n----Dang nhap----");
            printf("\n> Ten nguoi dung (go 'quit' de thoat): "); scanf("%s", username);
            if(strcmp(username, "quit") == 0)
                return 0;
            printf("> Mat khau: "); scanf("%s", password);
            strcpy(message, "lgi-");        // message
            strcat(message, username);      // message created in these 4 lines
            strcat(message, "-");           //
            strcat(message, password);      //
            send(sock, message, SEND_RECV_LEN , 0);
            state = WAITING_RESPONSE;
        }
    }
    return 1;
}

void requestLogout(int sock){
    char buff[100];
    strcpy(buff, "logout-");                // message created in these 2 lines
    strcat(buff, current_user->username);   //
    send(sock, buff, SEND_RECV_LEN, 0);
    state = WAITING_RESPONSE;
}

int requestSignup(int sock){
    char username[100];
    char password[100];
    char repassword[100];
    char message[256];
    while(state == NOT_LOGGED_IN || state == WAITING_RESPONSE){
        if(state == NOT_LOGGED_IN){
            while(1){
                system("clear");
                printf("\n----Dang ki----");
                printf("\n> Ten nguoi dung (go 'quit' de thoat): "); scanf("%s", username);
                if(strcmp(username, "quit") == 0)
                    return 0;
                printf("> Mat khau: "); scanf("%s", password);
                printf("> Xac nhan lai mat khau: "); scanf("%s", repassword);
                if(strcmp(password, repassword) == 0)
                    break;
                else printf(">> Xac nhan mat khau khong chinh xac. Vui long nhap lai.");
            }
            snprintf(message, sizeof(message), "SIGNUP-%s-%s", username, password);
            send(sock, message, SEND_RECV_LEN , 0);
            state = WAITING_RESPONSE;
        }
    }
    return 1;
}