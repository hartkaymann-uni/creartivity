#pragma once

#include "ccUser.h"


/// <summary>
/// Class for user storage and management. Singleton.
/// </summary>
class ccUserManager
{

public:
	/// <summary>
	/// Get the instance.
	/// </summary>
	/// <returns>Reference to the static instance of the user manager.</returns>
	static ccUserManager& get() {
		static ccUserManager instance;
		return instance;
	}

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
	ccUserManager();

	// Vector containing all users, first user is always the mouse
	map<int, ccUser> users;
	
	// Used to store and find mouse user
	int mouseUserId; 
};

