#pragma once
#include "ccScene.h"


class FluidScene : public ccScene
{

	const int NUM_PARTICLES = 1000;

public:
	FluidScene();
	void setup();
	void update();
	void draw();

private:
	vector<ofPoint> particles;

};

