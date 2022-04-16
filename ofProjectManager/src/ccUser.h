#pragma once

#include "ofMain.h"

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
	
	void move(glm::vec3 dir);
	
	bool setId( int id );
	void setPosition( glm::vec2 pos );
	void setPosition( glm::vec3 pos );
	void setPositions( glm::vec3 left, glm::vec3 right );
	
	pair<glm::vec3, glm::vec3> getPositons();
	pair<glm::vec3, glm::vec3> getMotions();

	glm::vec3& left = positions.first;
	glm::vec3& right = positions.second;


private:
	// Doesn't do anything yet, but important for future
	int id;

	pair<glm::vec3, glm::vec3> positions;
	pair<glm::vec3, glm::vec3> motions;
};

