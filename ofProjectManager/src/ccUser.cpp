#include "ccUser.h"

ccUser::ccUser() : id(-1) {
	positions = pair<glm::vec3, glm::vec3>( glm::vec3( 0.0 ), glm::vec3( 0.0 ) ); 
	velocities = pair<glm::vec3, glm::vec3>( glm::vec3( 0.0 ), glm::vec3( 0.0 ) );
};

ccUser::ccUser( int id_, glm::vec3 l, glm::vec3 r )
{
	id = id_;
	positions = pair<glm::vec3, glm::vec3>( l, r);
	motions = pair<glm::vec3, glm::vec3>( l, r);
	velocities = pair<glm::vec3, glm::vec3>( glm::vec3( 0.0 ), glm::vec3( 0.0 ) );
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

void ccUser::setTargetPositions( glm::vec3 left, glm::vec3 right )
{
	//positions.first = left;
	//positions.second = right;
	targetPositions.first = left;
	targetPositions.second = right;
}

void ccUser::moveTowardsTarget() {
	positions.first += calculateAcceleration(velocities.first, positions.first, targetPositions.first);
	positions.second += calculateAcceleration( velocities.second, positions.second, targetPositions.second );
}

glm::vec3 ccUser::calculateAcceleration(glm::vec3 currentVelocity, glm::vec3 origin, glm::vec3 target ) {
	glm::vec3 diff = target - origin;
	if ( diff == glm::vec3( 0 ) ) diff = glm::vec3( 0.01, 0, 0 );
	float distance = glm::length( diff );
	float distancePerentage = max( min( distance / 1, 1.f ), 0.f );
	float realAccel = BASE_ACCELERATION * distancePerentage * ofGetLastFrameTime();

	currentVelocity += glm::normalize( diff ) * realAccel;

	// Check if maxSpeed has been reached
	if ( glm::length( currentVelocity ) > MAX_MOVEMENT_SPEED ) {
		currentVelocity = glm::normalize( currentVelocity * MAX_MOVEMENT_SPEED );
	}

	return currentVelocity;
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