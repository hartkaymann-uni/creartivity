#include "ccUser.h"

ccUser::ccUser() : id(-1) {};

ccUser::ccUser( int id_, glm::vec3 l, glm::vec3 r )
{
	id = id_;
	positions = pair<glm::vec3, glm::vec3>( l, r);
	motions = pair<glm::vec3, glm::vec3>( l, r);
}

void ccUser::move( glm::vec3 vl, glm::vec3 vr )
{
	setPositions( positions.first + vl, positions.second + vr );
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

void ccUser::setMotions( glm::vec3 ml, glm::vec3 mr )
{
	motions.first = ml;
	motions.second = mr;
}

pair<glm::vec3, glm::vec3> ccUser::getPositons()
{
	return positions;
}

pair<glm::vec3, glm::vec3> ccUser::getMotions()
{
	return motions;
}