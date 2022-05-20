#pragma once

#include "ofMain.h"

#define MAX_MOVEMENT_SPEED 8.0f
#define BASE_ACCELERATION 3.0f

/*
	User Class
	Stores information on position and movements of a user.
	Can be used for mouse movement and for actual information.
*/

class ccUser
{
public:
	ccUser();
	ccUser( int id, glm::vec3 left, glm::vec3 right );

	void move( glm::vec3 vl, glm::vec3 vr );

	void setId( int id );
	void setPosition( glm::vec2 pos );
	void setPosition( glm::vec3 pos );
	void setPositions( glm::vec3 left, glm::vec3 right );
	void setTargetPositions( glm::vec3 left, glm::vec3 right );
	void setMotions( glm::vec3 ml, glm::vec3 mr);
	void moveTowardsTarget();

	inline int getId() { return id; };
	pair<glm::vec3, glm::vec3> getPositons();
	pair<glm::vec3, glm::vec3> getMotions();

	inline glm::vec3 left() { return positions.first; };
	inline glm::vec3 right() { return positions.second; };

private:
	// Doesn't do anything yet, but important for future
	int id;

	pair<glm::vec3, glm::vec3> positions;
	pair<glm::vec3, glm::vec3> targetPositions;
	pair<glm::vec3, glm::vec3> motions;
	pair<glm::vec3, glm::vec3> velocities;

	glm::vec3 calculateAcceleration(glm::vec3 currentVelocity, glm::vec3 origin, glm::vec3 target);
};

