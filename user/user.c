#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"

UserNode* createUserNode(char* username, char* passwd){
    // TODO
    UserNode* newnode = (UserNode*) malloc(sizeof(UserNode));
    strcpy(newnode->username, username);
    strcpy(newnode->password, passwd);
    newnode->status = OFFLINE;
    newnode->right = NULL;
    newnode->left = NULL;
    return newnode;
}

void traverseUserTree(UserNode* root){
    // TODO
    if(root == NULL) return;
    traverseUserTree(root->left);
    printf("\n<%s><%s><%d>", root->username, root->password, root->status);
    traverseUserTree(root->right);
}

UserNode* addUser(UserNode* root, char* username, char* passwd){
    // TODO
    if(root == NULL) root = createUserNode(username, passwd);
    else {
        UserNode* node = searchUser(root, username);
        if(node == NULL){
            if(strcmp(root->username, username) < 0) root->right = addUser(root->right , username, passwd);
            if(strcmp(root->username, username) > 0) root->left = addUser(root->left , username, passwd);
        }
    }
    return root;
}

UserNode* searchUser(UserNode* root, char* username){
    // TODO
    if(root == NULL) return NULL;
    else {
        if(strcmp(root->username, username) < 0)
            root = searchUser(root->right, username);
        else {
            if(strcmp(root->username, username) > 0)
                root = searchUser(root->left, username);
        }
    }
    return root;
}

int updateUserStatus(UserNode* root, char* username, UserStatus status){
    // TODO
    if(root == NULL) return -1;
    else{
        UserNode* node = searchUser(root, username);
        if(node != NULL){
            if(node->status == status) return 0;
            node->status = status;
        }
    }
    return 1;
}

void delUserBST(UserNode* root){
    // TODO
}