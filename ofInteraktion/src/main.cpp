/*
*  @author: Irene Santana Martin, Christine Schuller, Kay Hartmann, Cosmo Strattner, Marvin Esche, Franziska Streifert
* 
*  May 2022
*/

#include "ofMain.h"
#include "ofApp.h"


//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion( 3, 2 );	
	settings.setSize( 1920, 1080);
	settings.setPosition( glm::vec2( 3840 + 1024, 0 ) );
	settings.decorated = false;

	ofCreateWindow( settings );
	
	ofRunApp(new ofApp());
}
