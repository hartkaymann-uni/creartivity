#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
	ofGLFWWindowSettings settings;
	settings.setGLVersion( 3, 2 );
	settings.setSize( 640, 640 );
	settings.setPosition( ofVec2f(10, 100) );
	ofCreateWindow( settings );
	
	ofRunApp(new ofApp());

}
