#pragma once

#include "ccUser.h"

class ccUserManager
{
public:

	ccUserManager();

	void registerUser( ccUser& user );
	void removeUser( int id );

	vector<ccUser>* const getUsers();
	ccUser* const getUser( int id );
	ccUser* const getMouseUser();

private:

	// Vector containing all users, first user is always the mouse
	vector<ccUser> users;
};

