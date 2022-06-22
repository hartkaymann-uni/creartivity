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
	settings.setSize( 1080, 720);
	settings.setPosition( glm::vec2( 0, 0 ) );
	settings.decorated = false;

	ofCreateWindow( settings );
	
	ofRunApp(new ofApp());
}
