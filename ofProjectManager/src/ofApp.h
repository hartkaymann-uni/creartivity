#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"

#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
#include "ofxGui.h"
#include "ofxTransformPanel.h"
#endif

class ParticleScene;
class GameOfLifeScene;

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

	ParticleScene* particleScene;
	GameOfLifeScene* gameOfLifeScene;

#ifdef HAVE_OFX_GUI
	ofxTransformPanel panel;
#endif
};
