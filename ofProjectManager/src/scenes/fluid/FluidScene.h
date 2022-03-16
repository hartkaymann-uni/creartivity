#pragma once

/*
	Fluid Scene
	Simulation of an incompressible, homogenous fluid using the navier-stokes equaution.
	Right now the simulation is two-dimensional, but three-dimensional movement should be added in the future.
*/

#include "ccScene.h"

struct Particle;

class FluidScene : public ccScene
{

	const int NUM_PARTICLES = 1000;

public:
	FluidScene();
	void setup();
	void update();
	void draw();

private:
	vector<Particle> particles;

	ofTexture velocityGrid;

};

struct Particle {
	glm::vec2 pos;
	glm::vec2 vel;
	float p;
};

