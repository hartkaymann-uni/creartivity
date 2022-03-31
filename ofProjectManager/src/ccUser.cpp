#include "ccUser.h"

ccUser::ccUser() :
	id( 0 ),
	position( { 0.f, 0.f, 0.f } ),
	motion( { 0.f, 0.f, 0.f } )
{}

void ccUser::move( glm::vec3 dir )
{
	setPosition( position + dir);
}

bool ccUser::setId( int id ) {
	if (this->id != 0) {
		return false;
	} 

	this->id = id;
	return true;

}

void ccUser::setPosition( glm::vec3 pos )
{
	motion = pos - position;
	position = pos;

	//cout << "User:" << id << " Position:" << position << " Motion: " << motion << endl;
}

void ccUser::setPosition( glm::vec2 pos ) {
	setPosition( { pos.x, pos.y, position.z } );
}

glm::vec3 ccUser::getPositon()
{
	return position;
}

glm::vec3 ccUser::getMotion()
{
	return motion;
}