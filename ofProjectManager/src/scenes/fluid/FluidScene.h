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
	float time = 0.f;
	float dt = 0.f;
	float timestep = 1.f;
	bool debug = false;
	bool step = false;
	float splatRadius = 0.1;

	struct Grid {
		glm::vec2 size;
		float scale;
		bool applyBounds;
	};
	Grid grid = { {512, 256}, 1.f, true };
	ofPlanePrimitive plane;

	bool applyVorticity = true;
	bool applyViscosity = false;
	float viscosity = 0.3;
	float epsilon = 0.00024414;
	float curl = 0.3;
	int jacobiIterations = 20; // normal: 20-50
	float dissipation = 0.998; // none:1, slow:0.998, fast=0.992, veryfast=0.9

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

	// Vector Field
	ofFbo velocity;
	// Scalar Fields
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
	ofShader splatProgram;

	ofShader displayVectorProgram;

	void project();
	void addForces();

	// Slabops
	void advect( ofFbo& advected, ofFbo& output, float d );
	void boundary( ofFbo& input, ofFbo& output, float scale = -1.f );
	void boundarySide( ofFbo& input, ofFbo& output, ofPolyline& line, glm::vec2 offset, float scale );
	void vortex( ofFbo& output );
	void vortexConfine( ofFbo& vorticity, ofFbo& output );
	void diffuse( ofShader& jacobi, ofFbo& x, ofFbo& b, ofFbo& output, float alpha = -1.f, float beta = 4.f, float scale = -1.f );
	void diffuseStep( ofShader& jacobi, ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta );
	void diverge( ofFbo& divergence );
	void gradiate( ofFbo& output );
	void splat( ofFbo& read, glm::vec3 color, glm::vec2 point );

	void clearBuffer( ofFbo& buffer );

};
