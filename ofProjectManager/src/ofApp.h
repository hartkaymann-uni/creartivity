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

// OSC Receiver Settings, need to match settings in ofInteraction/ofInput
#define HOST "localhost"
#define PORT 12345

class ccScene;

/// <summary>
/// Called after application is startedand contains the main update and draw loop, as well as some event handlers
/// </summary>
class ofApp : public ofxApp {

public:
	void setup();
	void update();
	void draw();

	// Event Handlers
	void keyPressed( int key );
	void mouseDragged( int x, int y, int button );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void windowResized( int w, int h );

	// Scene Handling
	enum class SceneChangeType {
		Next,
		Previous,
		Last
	};
	void ChangeScene(SceneChangeType type);
	void CheckSceneTransitions();
	unsigned int GetSceneIndex(SceneChangeType type);
	float nextActionTime;
	void (ofApp::* nextAction)();


private:

	ofxTransformer transformer;

	bool showGui = true;

	ofImage img;

	ofxSceneManager sceneManager;
	float durationPerScene = 60.f;
	float lastSceneChangeTime;
	int lastScene;
	void NextScene();
	void PreviousScene();

	ccReceiver receiver;
	vector<ccScene*> scenes;
};
