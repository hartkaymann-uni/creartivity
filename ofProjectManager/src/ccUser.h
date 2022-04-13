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
	
	void move(glm::vec3 dir);
	
	bool setId( int id );
	void setPosition( glm::vec3 p );
	void setPosition( glm::vec2 p );

	glm::vec3 getPositon();
	glm::vec3 getMotion();


private:
	// Doesn't do anything yet, but important for future
	int id;

	glm::vec3 position;
	glm::vec3 motion;

};

