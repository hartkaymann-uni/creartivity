#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
private:
	ofShader shader;

	ofPlanePrimitive plane;
		
	ofTexture tex;
};
