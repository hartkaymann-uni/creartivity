#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {

	ofGLFWWindowSettings settings;
	settings.setPosition( ofVec2f( 660.0, 100.0 ) );
	settings.setGLVersion( 3, 2 );
	
	//settings.windowMode = OF_FULLSCREEN;
	settings.setSize( 1024, 768 );
	//settings.setSize( 2560, 1440 ); // WQHD
	
	//settings.decorated = false;
	settings.stencilBits = 8;
	ofCreateWindow( settings );
	

	ofRunAppWithAppUtils( new ofApp() );
}
