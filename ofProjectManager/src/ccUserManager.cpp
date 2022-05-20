#include "ccUserManager.h"

ccUserManager::ccUserManager()
{
	users.clear();
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

void ccUserManager::updateUserPositions() {

	for ( auto& u : users ) {
		u.second.moveTowardsTarget();
	}
}

vector<ccUser> const ccUserManager::getUserVec()
{
	vector<ccUser> v;
	for (const auto& u : users)
		v.push_back( u.second );

	return v;
}

vector<ofVec2f> ccUserManager::getHandsVec()
{
	vector<ofVec2f> v;
	for (auto& u : users) {
		v.push_back(u.second.left());
		v.push_back(u.second.right());
	}

	return v;
}

int ccUserManager::getUserCount() {
	return users.size();
}

ccUser* const ccUserManager::getUser( int id )
{
	return &(users.at( id ));
}

ccUser* const ccUserManager::getMouseUser()
{
	return &users[0];
}
