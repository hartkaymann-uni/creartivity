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
	map<int, ccUser>::iterator it = users.find( id );
	users.erase( it );
}

map<int, ccUser>* const ccUserManager::getUsers()
{
	return &users;
}

vector<ccUser> const ccUserManager::getUserVec()
{
	vector<ccUser> v;
	for (const auto& u : users)
		v.push_back( u.second );

	return v;
}

ccUser* const ccUserManager::getUser( int id )
{
	return &(users.at( id ));
}

ccUser* const ccUserManager::getMouseUser()
{
	return &users[0];
}