#include "ccUserManager.h"

ccUserManager::ccUserManager()
{
	// Set mouse as first user
	ccUser mouse;
	registerUser( mouse );

}

void ccUserManager::registerUser( ccUser& user )
{
	int id = users.size() + 1;

	if (user.setId( id )) {
		users.insert( pair<int, ccUser>( id, user ) );
	}
	else {
		string msg = "Could not set id of user! User already has an id.";
		ofLog( OF_LOG_WARNING, msg );
	}
}

void ccUserManager::removeUser( int id )
{
	users.erase( id );
}

map<int, ccUser>* const ccUserManager::getUsers()
{
	return &users;
}

ccUser* const ccUserManager::getUser( int id )
{
	return &(users.at( id ));
}

ccUser* const ccUserManager::getMouseUser()
{
	return &users[0];
}
