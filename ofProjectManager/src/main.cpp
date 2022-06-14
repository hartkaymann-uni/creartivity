#include "ofMain.h"
#include "ofApp.h"

#if 0
#define PRESENTATION_MODE
#endif 

#ifdef PRESENTATION_MODE
int SCREEN_WIDTH = 1920; // 3840
int SCREEN_HEIGHT = 1080; // 1080
#else
int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;
#endif 

//========================================================================
int main() {

	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
#ifdef PRESENTATION_MODE
	settings.setPosition(glm::vec2{ -(SCREEN_WIDTH - 1), 0 });
	settings.monitor = 1;
	settings.windowMode = OF_FULLSCREEN;
#endif

	//settings.decorated = false;
	settings.stencilBits = 8;
	ofCreateWindow(settings);

	ofRunAppWithAppUtils(new ofApp());
}
