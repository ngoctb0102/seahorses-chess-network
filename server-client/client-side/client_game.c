#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../message.h"
#include "client_user.h"

void startGame(int sock){
    send(sock, "STARTC", SEND_RECV_LEN, 0);
}