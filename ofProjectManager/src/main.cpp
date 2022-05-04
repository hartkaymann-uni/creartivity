#include "ofMain.h"
#include "ofApp.h"

//#define PRESENTATION_MODE

#ifdef PRESENTATION_MODE
int SCREEN_WIDTH = 3840;
int SCREEN_HEIGHT = 1080;
#else
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;
#endif 

//========================================================================
int main() {

	ofGLFWWindowSettings settings;
	settings.setGLVersion( 3, 2 );
	settings.setSize( SCREEN_WIDTH, SCREEN_HEIGHT );
#ifdef PRESENTATION_MODE
	//settings.multiMonitorFullScreen = true;
	settings.setPosition( ofVec2f( 0.0, 0.0 ) );
	settings.monitor = 2;
	isettings.windowMode = OF_FULLSCREEN;
#endif

	//settings.decorated = false;
	settings.stencilBits = 8;
	ofCreateWindow( settings );


	ofRunAppWithAppUtils( new ofApp() );
}
