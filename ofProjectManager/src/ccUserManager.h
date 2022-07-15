#pragma once

#include "ccUser.h"


/// <summary>
/// Class for user storage and management.Scenes should only store a reference to one instance (TODO: make singleton).
/// </summary>
class ccUserManager
{
public:

	ccUserManager();

	void updateUserPositions();
	void registerUser( ccUser& user );
	void removeUser( int id );

	void registerMouseUser();
	void removeMouseUser();

	map<int, ccUser>* const getUsers();
	vector<ccUser> const getUserVec();
	vector<ofVec2f> getHandsVec();
	ccUser* const getUser( int id );
	ccUser* const getMouseUser();
	int getUserCount();

private:

	// Vector containing all users, first user is always the mouse
	map<int, ccUser> users;
	int mouseUserId; // Used to store and find mouse user
};

