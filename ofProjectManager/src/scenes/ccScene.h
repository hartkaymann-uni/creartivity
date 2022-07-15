#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxGui.h"

#include "ccReceiver.h"
#include "ccUserManager.h"

#define HOST "localhost"
#define PORT 12345

#define MAX_USERS 10


/// <summary>
/// Parent class for all other scenes. Includes some utilitiies, variables and virtual methods.
/// </summary>
class ccScene : public ofxScene
{
public:

	ccScene( std::string name = "Unnamed Scene" );

	virtual void setup() {};
	virtual void update() {};
	virtual void draw() {};

	void keyPressed( int key );
	void keyReleased( int key );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseDragged( int x, int y, int button );
	void windowResized( int w, int h ); 
	
	enum class CoordinateSystem {
		Normals,
		Screen,
		World
	};

	inline void setUserManager( ccUserManager* um ) { userManager = um; }
	inline ofxPanel& getGui() { return gui; };
	vector<ofVec3f> getHandPositions(CoordinateSystem system);

	// Scene Handling
	virtual float SceneIntro();
	virtual float SceneOutro();

protected:

	bool mouseIsDown;
	ofVec3f mousePosition;

	filesystem::path scenesPath;

	int width, height;

	ofEasyCam camera;
	ofxPanel gui;

	ccUserManager* userManager;

	void resetCamera();
	ofVec3f getProjectedPosition( ofVec3f mp );
	ofVec3f getNormalToScreenPosition(ofVec2f normalPos);
	ofVec3f getScreenToWorldPosition(ofVec2f screenPos);
	ofVec3f getNormalToWorldPosition(ofVec2f normalPos);

	bool isInBounds( ofVec2f pos );
	bool isInBounds( int x, int y );

	filesystem::path getCurrentPath();
	filesystem::path getShaderPath();
};

