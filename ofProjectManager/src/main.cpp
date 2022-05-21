#include "ofMain.h"
#include "ofApp.h"

#if 0
#define PRESENTATION_MODE
#endif 

#ifdef PRESENTATION_MODE
int SCREEN_WIDTH = 3840; // 3840
int SCREEN_HEIGHT = 1080; // 1080
#else
int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 820;
#endif 

//========================================================================
int main() {

	ofGLFWWindowSettings settings;
	settings.setPosition( ofVec2f( 660.0, 100.0 ) );
	settings.setGLVersion( 3, 2 );
	settings.setSize( SCREEN_WIDTH, SCREEN_HEIGHT );
#ifdef PRESENTATION_MODE
	//settings.multiMonitorFullScreen = true;
	settings.setPosition( ofVec2f( 0.0, 0.0 ) );
	settings.monitor = 2;
	settings.windowMode = OF_FULLSCREEN;
#endif

	//settings.decorated = false;
	settings.stencilBits = 8;
	ofCreateWindow( settings );
	

	ofRunAppWithAppUtils( new ofApp() );
}
