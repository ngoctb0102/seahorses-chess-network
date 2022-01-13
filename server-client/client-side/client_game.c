#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../message.h"
#include "client_user.h"

void startGame(int sock){
    send(sock, "STARTC", SEND_RECV_LEN, 0);
    // while(1){
    //     printf("abc");
    // }
    // while(state == IN_ROOM || state == WAITING_RESPONSE){
    //     if(state == IN_ROOM && room_updating == 0){
    //     // if(state == IN_ROOM && room_updating == 0){
    //         scanf("%d%*c", &choice);
    //         switch(choice){
    //             case 1: 
    //                 startGame(sock);
    //                 // game(sock);
    //                 break;
    //             case 2: exitRoom(sock); break;
    //             default: printf("\nKhong ro cau lenh.\n"); break;
    //         }
    //         if(choice == 2) break;
    //     }
    // }
}