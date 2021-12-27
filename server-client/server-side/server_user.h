#ifndef __SERVERUSER_H__
#define __SERVERUSER_H__

#include "../../user/user.h"

//-------------Globals----------------
extern UserNode* users;

//------------Functions---------------

void login(char** msg, int socket);

void logout(char** msg, int sock);

void signup(char** msg, int sock);

#endif