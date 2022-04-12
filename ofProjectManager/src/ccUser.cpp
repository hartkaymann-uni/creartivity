#include "ccUser.h"

ccUser::ccUser() 
{
};

ccUser::ccUser( int id, glm::vec3 l, glm::vec3 r )
{
	positions = pair<glm::vec3, glm::vec3>( l, r);
	motions = pair<glm::vec3, glm::vec3>( l, r);

	left = &positions.first;
	right = &positions.second;
}

bool ccUser::setId( int id ) {
	if (this->id != 0) {
		return false;
	} 

	this->id = id;
	return true;

}

void ccUser::setPosition( glm::vec2 pos ) {
	setPosition( { pos.x, pos.y, 0.f } );
}

void ccUser::setPosition(glm::vec3 pos) {
	setPositions( pos, pos );
}

void ccUser::setPositions( glm::vec3 left, glm::vec3 right)
{
	motions.first = left - positions.first;
	motions.second = right - positions.second;
	positions.first = left;
	positions.second = right;
}

pair<glm::vec3, glm::vec3> ccUser::getPositons()
{
	return positions;
}

pair<glm::vec3, glm::vec3> ccUser::getMotions()
{
	return motions;
}