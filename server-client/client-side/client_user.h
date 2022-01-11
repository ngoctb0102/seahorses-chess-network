#ifndef __CLIENTUSER_H__
#define __CLIENTUSER_H__

#include "../../user/user.h"

//--------------Globals--------------------
extern char current_user_name[100];
extern UserNode* current_user;
extern int state;

//--------------Functions-------------------

int request_login(int sock);

void request_logout(int sock);

int request_signup(int sock);

#endif