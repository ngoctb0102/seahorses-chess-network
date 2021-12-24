#ifndef __USER_H__
#define __USER_H__

// User status
typedef enum {
    OFFLINE,
    ONLINE,
    INGAME
} UserStatus;

// User node for User BST
typedef struct UserNode {
    char username[100];
    char password[100];
    UserStatus status;
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