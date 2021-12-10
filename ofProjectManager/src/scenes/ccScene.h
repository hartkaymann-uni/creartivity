#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

#define MAX_USERS 10

struct user {
	int id;
	ofVec2f positionLeft;
	ofVec2f positionRight;
};

class ccScene : public ofxScene
{
public:

	ccScene( std::string name = "Unnamed Scene" );

	inline ofxOscReceiver* getReceiver() const { return receiver; }
	inline void setReceiver( ofxOscReceiver* r ) { receiver = r; }

protected:

	int width, height;

	ofEasyCam camera;
	ofxOscReceiver* receiver;

	std::map<int, user> users;
	ofVec2f user_positions[MAX_USERS];

	string connectionStatusString = "Status: off";

	void receiveMessage();
	void resetCamera();
	ofVec3f getProjectedPosition( ofVec3f mp );

};

