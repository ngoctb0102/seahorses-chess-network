#ifndef __CLIENTUSER_H__
#define __CLIENTUSER_H__

//--------------Globals--------------------
extern char current_user_name[100];

//--------------Functions-------------------

int login(int sock);

void logout(int sock);

int signup(int sock);

#endif