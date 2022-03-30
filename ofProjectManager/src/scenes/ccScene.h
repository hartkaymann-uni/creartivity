#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxGui.h"

#include "ccReceiver.h"
#include "ccUserManager.h"

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

	void keyPressed( int key );
	void keyReleased( int key );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseDragged( int x, int y, int button );
	void windowResized( int w, int h );

	inline void setReceiver( ccReceiver* r ) { receiver = r; }
	inline void setUserManager( ccUserManager* um ) { userManager = um; }
	inline ofxPanel& getGui() { return gui; };

protected:

	bool mouseIsDown;
	ofVec3f mousePosition;

	filesystem::path scenesPath;

	int width, height;
	array<ofVec2f, MAX_USERS> user_positions;

	ofEasyCam camera;
	ofxPanel gui;

	ccReceiver* receiver;
	ccUserManager* userManager;

	void resetCamera();
	void updateUserPositions();
	ofVec3f getProjectedPosition( ofVec3f mp );

	bool isInBounds( ofVec2f pos );
	bool isInBounds( int x, int y );

	filesystem::path getCurrentPath();
	filesystem::path getShaderPath();
};

