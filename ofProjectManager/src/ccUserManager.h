#pragma once

#include "ccUser.h"

class ccUserManager
{
public:

	ccUserManager();

	void registerUser( ccUser& user );
	void removeUser( int id );

	map<int, ccUser>* const getUsers();
	vector<ccUser> const getUserVec();
	ccUser* const getUser( int id );
	ccUser* const getMouseUser();

private:

	// Vector containing all users, first user is always the mouse
	map<int, ccUser> users;
};

