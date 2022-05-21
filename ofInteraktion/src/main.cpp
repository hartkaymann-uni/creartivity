/*
*  @author: Irene Santana Martin, Christine Schuller, Kay Hartmann, Cosmo Strattner, Marvin Esche, Franziska Streifert
* 
*  May 2022
*/

#include "ofMain.h"
#include "ofApp.h"

#if 1
#define PRESENTATION_MODE
#endif 

#ifdef PRESENTATION_MODE
int SCREEN_WIDTH = 1920; // 3840
int SCREEN_HEIGHT = 1080; // 1080
#else
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 640;
#endif 

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion( 3, 2 );	
	settings.setSize( SCREEN_WIDTH, SCREEN_HEIGHT );
#ifdef PRESENTATION_MODE
	//settings.multiMonitorFullScreen = true;
	settings.monitor = 2;
	settings.windowMode = OF_FULLSCREEN;
#endif
	ofCreateWindow( settings );
	
	ofRunApp(new ofApp());
}
