#pragma once
#include "ccScene.h"
class IntroScene :
    public ccScene
{
public:
	IntroScene();

	void setup();
	void update();
	void draw();

private:

	struct Particle {
		ofVec2f pos;
		float r;
		float theta;
	};

	float time;

	float radius = 100;

	vector<Particle> particles;

};

