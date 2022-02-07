#pragma once

#include "ofMain.h"

class ccUser
{
public:

	ccUser( ofVec3f pos = { 0, 0, 0 } );

	void move();
	void setTarget( ofVec3f target );

	ofVec3f pos;
	ofVec3f vel;

	ofVec3f targetPos;

	float speed;


};

