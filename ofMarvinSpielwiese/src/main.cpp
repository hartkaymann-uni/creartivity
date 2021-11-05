#include "ofMain.h"
#include "ofApp.h"
#include "ofRandomLines.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	if(0) ofRunApp(new ofApp());
	if(1) ofRunApp(new ofRandomLines());
}
