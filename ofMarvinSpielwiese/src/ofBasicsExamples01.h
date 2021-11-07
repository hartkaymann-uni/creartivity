#pragma once

#include "ofMain.h"

class ofBasicsExamples01 : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void drawWaves();
	void drawClock();

	ofTrueTypeFont font;
};