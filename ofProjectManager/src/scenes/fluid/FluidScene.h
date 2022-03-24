#pragma once

/*
	Fluid Scene
	Simulation of an incompressible, homogenous fluid using the navier-stokes equaution.
	Right now the simulation is two-dimensional, but three-dimensional movement should be added in the future.
*/

#include "ccScene.h"

struct Particle {
	glm::vec2 pos;
};

class FluidScene : public ccScene
{

	const int NUM_PARTICLES = 1000;
	const int NUM_CELLS = 1000;

public:
	FluidScene();
	void setup();
	void update();
	void draw();

private:
	float time;

	vector<Particle> particles;

	int cells;
	int grid_width, grid_height;

	ofFbo velocityGrid;
	ofFbo pressureGrid;
	ofFbo vorticityGrid;
	ofFbo forceGrid;
	ofFbo inkGrid;

	ofShader advectProgram;
	ofShader jacobiProgram;
	ofShader forceProgram;
	ofShader divergenceProgram;
	ofShader gradientProgram;
	ofShader boundariesProgram;

	void calculateGridDimensions();

};
