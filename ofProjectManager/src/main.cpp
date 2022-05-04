#include "ofMain.h"
#include "ofApp.h"

int SCREEN_WIDTH = 3840;
int SCREEN_HEIGHT = 1080;

//========================================================================
int main() {

	ofGLFWWindowSettings settings;
	settings.setGLVersion( 3, 2 );
	settings.setSize( SCREEN_WIDTH, SCREEN_HEIGHT);
	settings.setPosition( ofVec2f( 0.0, 0.0 ) );
	settings.windowMode = OF_FULLSCREEN;
	//settings.multiMonitorFullScreen = true;
	settings.monitor = 2;
	//settings.decorated = false;
	settings.stencilBits = 8;
	ofCreateWindow( settings );


	ofRunAppWithAppUtils( new ofApp() );
}
