#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxOsc.h"

#include "ccReceiver.h"

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

	ofxTransformer transformer;
	ofxSceneManager sceneManager;
	int lastScene;

	ccReceiver receiver;
	vector<ccScene*> scenes;

#ifdef HAVE_OFX_GUI
	ofxTransformPanel panel;
#endif

};
