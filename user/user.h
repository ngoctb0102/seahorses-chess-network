#ifndef __USER_H__
#define __USER_H__
#define LEN 100

// User status
typedef enum {
    OFFLINE,
    ONLINE,
    INGAME
} UserStatus;

// User node for User BST
typedef struct UserNode {
    char username[LEN];
    char password[LEN];
    UserStatus status;
    int room_id;
    int send_sock;
    int recv_sock;
    struct UserNode *left, *right;
} UserNode;

// User BST utils
void traverseUserTree(UserNode* root);
UserNode* searchUser(UserNode* root, char* username);
UserNode* addUser(UserNode* root, char* username, char* passwd);
int updateUserStatus(UserNode*, char* username, UserStatus status);
void delUserBST(UserNode* root); // unused when server is online, just to safely delete BST when shutdown server.
UserNode* createUserNode(char* username, char* passwd);
// Data write util


#endif