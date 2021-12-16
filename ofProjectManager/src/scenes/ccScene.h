#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxGui.h"

#include "ccReceiver.h"

#define HOST "localhost"
#define PORT 12345

#define MAX_USERS 10

class ccScene : public ofxScene
{
public:

	ccScene( std::string name = "Unnamed Scene" );

	virtual void setup();
	virtual void update();
	virtual void draw();

	inline void setReceiver( ccReceiver* r ) { receiver = r; }
	inline ofxPanel& getGui() { return gui; };

protected:
	int width, height;
	ofVec2f user_positions[MAX_USERS];

	ofEasyCam camera;
	ofxPanel gui;

	ccReceiver* receiver;

	void resetCamera();
	void updateUserPositions();
	ofVec3f getProjectedPosition( ofVec3f mp );

	bool isInBounds( int x, int y );

};

