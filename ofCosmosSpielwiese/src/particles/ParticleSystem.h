#pragma once
#include "DemoParticle.h"


class ParticleSystem
{
public:
	void setup();
	void update();
	void draw();
	void resetParticles();
	void ChangeAttractionMode(int key);
	void ColorExplosion(ofColor color, glm::vec2 source);
	void Impulse(glm::vec2 direction, glm::vec2 source);
	void AfterModeSwap();

	particleMode currentMode;
	string currentModeStr;

	vector <demoParticle> p;
	vector <glm::vec3> attractPoints;
	vector <glm::vec3> attractPointsWithMovement;

};

