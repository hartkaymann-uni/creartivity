#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {

	ofGLFWWindowSettings settings;
	settings.setGLVersion( 3, 2 );
	settings.setSize( 1024, 768 );
	settings.setPosition( ofVec2f( 660.0, 100.0 ) );
	//settings.decorated = false;
	settings.stencilBits = 8;
	ofCreateWindow( settings );


	ofRunAppWithAppUtils( new ofApp() );
}
