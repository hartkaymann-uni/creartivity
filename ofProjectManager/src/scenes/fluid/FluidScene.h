#pragma once

/*
	Fluid Scene
	Simulation of an incompressible, homogenous fluid using the navier-stokes equaution.
	Right now the simulation is two-dimensional, but three-dimensional movement should be added in the future.
*/

#include "ccScene.h"
#include <time.h>

class FluidScene : public ccScene
{
public:
	FluidScene();
	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );

private:
	float time;
	float dt;
	bool debug;
	bool step;

	struct Grid {
		glm::vec2 size;
		float scale;
		bool applyBounds;
	};

	Grid grid;

	float displayScalar;
	float displayVector;

	float viscosity = 0.3;
	bool applyViscosity = false;
	bool applyVorticity = false;

	float epsilon = 0.00024414;
	float curl = 0.3;

	float jacobiIterations = 50;

	float dissipation = 0.998; // none:1, slow:0.998, fast=0.992, veryfast=0.9

	ofFbo velocity;
	ofFbo density;
	ofFbo velocityDivergence;
	ofFbo velocityVorticity;
	ofFbo pressure;

	ofShader advectProgram;
	ofShader gradientProgram;
	ofShader jacobiscalarProgram;
	ofShader jacobivectorProgram;
	ofShader divergenceProgram;
	ofShader vorticityProgram;
	ofShader vorticityforceProgram;
	ofShader boundariesProgram;

	// Slabops
	void advect( ofFbo& advected, ofFbo& output );
	void boundary( ofFbo& input, ofFbo& output, float scale = -1.f);
	void vortex( ofFbo& output );
	void vortexConfine( ofFbo& vorticity, ofFbo& output );
	void diffuse( ofFbo& x, ofFbo& b, ofFbo& output, float alpha = -1.f, float beta = 4.f, float scale = -1.f);
	void diffuseStep( ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta);
	void project();

};
