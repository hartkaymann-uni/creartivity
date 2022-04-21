#include "ccUser.h"

ccUser::ccUser() : id(-1) {};

ccUser::ccUser( int id_, glm::vec3 l, glm::vec3 r )
{
	id = id_;
	positions = pair<glm::vec3, glm::vec3>( l, r);
	motions = pair<glm::vec3, glm::vec3>( l, r);
}

void ccUser::move( glm::vec3 dir )
{
	setPosition( positions.first + dir);
}

void ccUser::setId( int id_ ) {
	id = id_;
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