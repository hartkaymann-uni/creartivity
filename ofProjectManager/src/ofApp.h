#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxOsc.h"

#include "ccReceiver.h"
#include "ccUserManager.h"

#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
#include "ofxGui.h"
#include "ofxTransformPanel.h"
#endif

#define HOST "localhost"
#define PORT 12345

class ccScene;

class ofApp : public ofxApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseEntered( int x, int y );
	void mouseExited( int x, int y );
	void windowResized( int w, int h );
	void dragEvent( ofDragInfo dragInfo );
	void gotMessage( ofMessage msg );

	//Scene Handling
	enum class SceneChangeType {
		Next,
		Previous,
		Last
	};

	void ChangeScene(SceneChangeType type);
	void CheckSceneTransitions();
	unsigned int GetSceneIndex(SceneChangeType type);
	//-----

	float nextActionTime;
	void (ofApp::* nextAction)();

	ofxTransformer transformer;
	ofxSceneManager sceneManager;
	int lastScene;

	ccReceiver receiver;
	vector<ccScene*> scenes;

private:
	void NextScene();
	void PreviousScene();

	bool showGui = true;

	ccUserManager userManager;
};
