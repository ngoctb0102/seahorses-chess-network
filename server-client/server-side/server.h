#ifndef __SERVER_H__
#define __SERVER_H__

#define MAX_CLIENT_ALLOWED 20

typedef struct {
    int client_send_sock;
    int client_recv_sock;
} ThrdHandlerArgs;

#define SERVER_PORT 9999
#define SERVER_ADDR "192.168.56.102"

#endif