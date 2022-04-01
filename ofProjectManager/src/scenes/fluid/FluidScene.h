#pragma once

#include "ccFluidSolver.h"

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

	bool debug = false;
	bool step = false;

	ofParameterGroup groupGeneral;
	ofParameterGroup groupView;
	ofParameterGroup groupBounds;
	ofParameterGroup groupViscosity;
	ofParameterGroup groupVorticity;
	ofParameterGroup groupSolver;

	ofParameter<float> p_Timestep;
	ofParameter<float> p_SplatRadius;
	ofParameter<bool> p_DebugView;
	ofParameter<bool> p_Bounds;
	ofParameter<bool> p_ApplyViscosity;
	ofParameter<float> p_Viscosity;
	ofParameter<bool> p_ApplyVorticity;
	ofParameter<float> p_Epsilon;
	ofParameter<float> p_Curl;
	ofParameter<int> p_JacobiIterations;
	ofParameter<float> p_Dissipation;

	ccFluidSolver solver;

	ofShader displayVectorProgram;

};
