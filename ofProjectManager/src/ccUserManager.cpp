#include "ccUserManager.h"

ccUserManager::ccUserManager()
{
	// Set mouse as first user
	ccUser mouse;
	mouse.setId( 0 );
	registerUser( mouse );

}

void ccUserManager::registerUser( ccUser& user )
{
	int id = user.getId();
	users.insert( pair<int, ccUser>( id, user ) );

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
