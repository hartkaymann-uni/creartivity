#pragma once

#include "ofMain.h"

class ofRandomLines : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void drawRandomLines();

	ofPolyline myLine;
};
