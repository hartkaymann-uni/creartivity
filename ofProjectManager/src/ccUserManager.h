#pragma once

#include "ccUser.h"
#include "ccReceiver.h"

class ccUserManager
{
public:

	ccUserManager();
	ccUserManager(ccReceiver& receiver);

	void registerUser( ccUser& user );
	void removeUser( int id );

	vector<ccUser>* const getUsers();
	ccUser* const getUser( int id );
	ccUser* const getMouseUser();

private:

	// Vector containing all users, first user is always the mouse
	map<int, ccUser> users;
	ccReceiver receiver;
};

