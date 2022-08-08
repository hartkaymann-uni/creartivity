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
	settings.setSize( 760, 480 );
	settings.setPosition( glm::vec2( 50, 50 ) );
	settings.decorated = true;

	ofCreateWindow( settings );
	
	ofRunApp(new ofApp());
}
