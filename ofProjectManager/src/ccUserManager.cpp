#include "ccUserManager.h"

ccUserManager::ccUserManager() : 
	mouseUserId( -1 )
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
	if( it != users.end() )
		users.erase( it );
}

void ccUserManager::registerMouseUser()
{
	ccUser mouseUser;
	mouseUserId = getUserCount() + (int) ofRandom(1000); // set mouse id to a random value
	mouseUser.setId( mouseUserId );
	registerUser( mouseUser );
}

void ccUserManager::removeMouseUser()
{
	removeUser( mouseUserId );
	mouseUserId = -1;
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
	if ( !users.count( id ) ) 
		return NULL;
	return &(users.at( id ));
}

ccUser* const ccUserManager::getMouseUser()
{
	if ( mouseUserId == -1 ) return NULL;
	return &users[mouseUserId];
}
