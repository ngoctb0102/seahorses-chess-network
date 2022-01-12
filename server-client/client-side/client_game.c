#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../message.h"

void startGame(int sock){
    send(sock, "STARTC", SEND_RECV_LEN, 0);
}