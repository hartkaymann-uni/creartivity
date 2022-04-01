#pragma once

#include "ofMain.h"
#include "ccUser.h"

class ccFluidSolver
{
public:
	struct Settings {
		float timestep; // 1.0
		float splatRadius = 0.1; // 0.1
		bool applyVorticity = true;
		bool applyViscosity = false;
		float viscosity = 0.3; // 0.3
		float epsilon = 0.00024414; // 0.00024414
		float curl = 0.3;	// 0.3
		int jacobiIterations = 20; // normal: 20-50
		float dissipation = 0.998; // none:1, slow:0.998, fast=0.992, veryfast=0.9
	};

	struct Grid {
		glm::vec2 size;
		float scale = 1.f;
		bool applyBounds;
	};

	ccFluidSolver();
	ccFluidSolver( Settings& settings );

	void setup( Grid& grid );
	void step( ccUser& user );

	ofFbo* getDensity();

	inline void setCurl( float& c ) { s.curl = c; }
	inline void setEpsilon( float& e ) { s.epsilon = e; }
	inline void setTimestep( float& t ) { s.timestep = t; }
	inline void setViscosity( float& v ) { s.viscosity = v; }
	inline void setBounds( bool& b ) { grid.applyBounds = b; }
	inline void setDissipation( float& d ) { s.dissipation = d; }
	inline void setSplatRadius( float& r ) { s.splatRadius = r; }
	inline void setApplyViscosity( bool& v ) { s.applyViscosity = v; }
	inline void setApplyVorticity( bool& v ) { s.applyVorticity = v; }
	inline void setJacobiIterations( float& i ) { s.jacobiIterations = i; }

private:
	ofFbo createFbo();

	void project();
	void addForces( ccUser& user );

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

	Settings s;
	Grid grid;

	ofPlanePrimitive plane;

	// Vector Field
	ofFbo velocity;
	// Scalar Fields
	ofFbo density;
	ofFbo divergence;
	ofFbo vorticity;
	ofFbo pressure;

	// Solver shaders
	ofShader advectProgram;
	ofShader gradientProgram;
	ofShader jacobiscalarProgram;
	ofShader jacobivectorProgram;
	ofShader divergenceProgram;
	ofShader vorticityProgram;
	ofShader vorticityforceProgram;
	ofShader boundariesProgram;
	ofShader splatProgram;
};

