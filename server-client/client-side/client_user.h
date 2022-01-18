#ifndef __CLIENTUSER_H__
#define __CLIENTUSER_H__

#include "../../user/user.h"

//--------------Globals--------------------
extern char current_user_name[100];
extern UserNode* current_user;
extern int state;

//--------------Functions-------------------

int requestLogin(int sock);

void requestLogout(int sock);

int requestSignup(int sock);

#endif