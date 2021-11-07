#pragma once

#include "ofMain.h"

class ofRandomLines : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	void drawPointAtCurvedLine();
	void drawCurvedPolyLine();
	void drawRandomPolyLine();
	void drawRandomLines();

	ofPolyline myLine;
	float linePercent;
};
