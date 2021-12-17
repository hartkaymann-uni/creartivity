#pragma once
#include "ofMain.h"
#include "ofxGui.h"

enum particleMode {
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE,
	PARICLE_MODE_FLOW_1
};

class demoParticle {

public:
	demoParticle();

	void setMode(particleMode newMode);
	void setAttractPoints(vector <glm::vec3>* attract);

	void reset();
	void update();
	void draw();
	void ColorExplosion(ofColor color, glm::vec2 source);
	void Impulse(glm::vec2 direction, glm::vec2 source);

	glm::vec2 pos;
	glm::vec2 vel;
	glm::vec2 frc;

	float drag;
	float uniqueVal;
	float scale;

	float spawnScaleMod;
	float spawnVelocityMod;

	ofColor particleColor;

	particleMode mode;

	static ofParameter<float> scaleMod;

	vector <glm::vec3>* attractPoints;
};
