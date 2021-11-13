#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.setGLVersion( 4, 3 ); // compute shaders supported since openGL 4.3
	settings.windowMode = OF_WINDOW;
	settings.setSize( 1024, 768 );
	ofCreateWindow( settings );			


	ofRunApp( new ofApp() );
}
