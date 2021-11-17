#pragma once

#include "ofMain.h"

class ofNNviaMesh : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void drawLightning(ofVec3f Pos1, ofVec3f Pos2, float displace = 10);

	void keyPressed(int key);

	ofMesh mesh;

	float speed;
	float ammoutOfPoints;
	float conectionDistance;
	bool visualizePoints = true;

};