#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

class ccScene : public ofxScene
{
	struct user {
		ofVec2f positionLeft;
		ofVec2f positionRight;
	};

public:
	ccScene(std::string name = "Unnamed Scene");

	inline ofxOscReceiver* getReceiver() const { return receiver; }
	inline void setReceiver( ofxOscReceiver* r ) { receiver = r; }

protected:

	int width, height;

	ofEasyCam camera;
	ofxOscReceiver* receiver;
	
	std::map<int, user> users;

	void receiveUsers();
	void resetCamera();
	ofVec3f getProjectedMousePosition( ofVec3f mp );

};

