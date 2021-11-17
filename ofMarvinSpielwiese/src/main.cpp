#include "ofMain.h"
#include "ofApp.h"
#include "ofRandomLines.h"
#include "ofBasicsExamples01.h"
#include "ofNNviaMesh.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	if (1) ofRunApp(new ofApp());
	//if (0) ofRunApp(new ofNNviaMesh());
	//if (0) ofRunApp(new ofRandomLines());
	//if (0) ofRunApp(new ofBasicsExamples01());
}
