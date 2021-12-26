#include <stdio.h>
#include <stdlib.h>
#include "user.h"

void main(){
    UserNode* root = NULL;
    FILE* fp = fopen("../accounts.txt", "r");
    if(fp == NULL){
        printf("\nKhong mo duoc file\n");
        return;
    }
    int account_no;
    char username[100];
    char passwd[100];
    fscanf(fp, "%d\n", &account_no);
    for(int i = 1; i <= account_no; i++){
        fscanf(fp, "%s %s\n", username, passwd);
        root = addUser(root, username, passwd);
    }
    traverseUserTree(root);
    printf("\n");
    fclose(fp);
    UserNode* node = searchUser(root, "tuanvu");
    if(node != NULL)
        printf("\nSearch: %s %s %d", node->username, node->password, node->status);
    updateUserStatus(root, "tuanvu", ONLINE);
    node = searchUser(root, "tuanvu");
    if(node != NULL)
        printf("\nSearch: %s %s %d\n", node->username, node->password, node->status);
}