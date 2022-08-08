#pragma once

#include "ofMain.h"
#include "ccUser.h"

#include "utils/ccPingPong.h"

/*
	Fluid Solver
	Simulates two-dimensional fluid based on an eulerian (grid-based) approach to solving the Navier-Stokes Equations
	Logic is done in fragment shaders and applied to vector or scalar fields represented by textures (Field struct)
	Vorticity and viscosity can be applied as well
*/

namespace fluid {

	/// <summary>
	/// Fluid Solver
	///	Simulates two-dimensional fluid based on an eulerian (grid-based) approach to solving the Navier-Stokes Equations
	///	Logic is done in fragment shaders and applied to vector or scalar fields represented by textures (Field struct)
	///	Vorticity and viscosity can be applied as well
	/// </summary>
	class ccSolver
	{
	public:
		// Solver settings
		struct Settings {
			float timestep; // 1.0
			float splatRadius = 0.1; // 0.1
			ofFloatColor splatColor;
			bool applyVorticity = true;
			bool applyViscosity = false;
			float viscosity = 0.3; // 0.3
			float epsilon = 0.00024414; // 0.00024414
			float curl = 0.3;	// 0.3
			int jacobiIterations = 20; // normal: 20-50
			float dissipation = 0.998; // none:1, slow:0.998, fast=0.992, veryfast=0.9
			bool applyGravity = false;
			glm::vec2 gravityDir = glm::vec2( 0.0, 1.0 );
			float gravityStr = 9.81; // 9.81
			bool splatDensity = true; // determines whetether to add to density on interaction
		};

		// Grid defining resolution of the solver
		struct Grid {
			glm::vec2 size;
			float scale = 1.f;
			bool applyBounds;
		};

		ccSolver();
		ccSolver( Settings& settings );

		void setup( Grid& grid );
		/// <summary>
		/// 
		/// </summary>
		/// <param name="users"></param>
		void step( vector<ccUser> users);
		void reset();

		// Getter
		inline Grid* const getGrid() { return &grid; }
		inline ofFbo* const getDensity() { return density.read; }
		inline ofFbo* const getVelocity() { return velocity.read; }
		inline ofFbo* const getDivergence() { return divergence.read; }
		inline ofFbo* const getVorticity() { return vorticity.read; }
		inline ofFbo* const getPressure() { return pressure.read; }

		// Setter
		inline void setTimestep( float t ) { s.timestep = t; }
		inline void setSplatRadius( float r ) { s.splatRadius = r; }
		inline void setSplatColor( ofFloatColor c ) { s.splatColor = c; }
		inline void setDissipation( float d ) { s.dissipation = d; }
		inline void setBounds( bool b ) { grid.applyBounds = b; }
		inline void setScale( float s ) { grid.scale = s; }
		inline void setJacobiIterations( int i ) { s.jacobiIterations = i; }
		inline void setApplyVorticity( bool v ) { s.applyVorticity = v; }
		inline void setEpsilon( float e ) { s.epsilon = e; }
		inline void setCurl( float c ) { s.curl = c; }
		inline void setApplyViscosity( bool v ) { s.applyViscosity = v; }
		inline void setViscosity( float v ) { s.viscosity = v; }
		inline void setApplyGravity( bool g ) { s.applyGravity = g; }
		inline void setGravityDirection( glm::vec2 d ) { s.gravityDir = d; }
		inline void setGravityStrength( float g ) { s.gravityStr = g; }
		inline void setSplatDensity( bool d ) { s.splatDensity= d; }

		void colorDensity( ofShader& shader );

	private:
		/// <summary>
		/// Allocate FBO with specified formet and set some basic properties
		/// </summary>
		/// <param name="format">Format of the FBO</param>
		/// <returns>Specified FBO with its texture's pixel scaling set to NEAREST and clamping set to CLAMP_TO_EDGE  </returns>
		ofFbo createFbo( int format );

		void project();
		void addForces( ccUser& user );

		// Slabops
		void advect( PingPong& advected, PingPong& output, float d );
		void boundary( PingPong& input, PingPong& output, float scale = -1.f );
		void boundarySide( PingPong& input, PingPong& output, ofPolyline& line, glm::vec2 offset, float scale );
		void vortex( PingPong& output );
		void vortexConfine( PingPong& vorticity, PingPong& output );
		void diffuse( ofShader& jacobi, PingPong& x, PingPong& b, PingPong& output, float alpha = -1.f, float beta = 4.f, float scale = -1.f );
		void diffuseStep( ofShader& jacobi, PingPong& x, PingPong& b, PingPong& output, float alpha, float beta );
		void diverge( PingPong& divergence );
		void gradiate( PingPong& output );
		void splat( PingPong& read, glm::vec3 color, glm::vec2 point );
		void gravitate( PingPong& output );

		Settings s;
		Grid grid;

		ofPlanePrimitive plane;

		// Vector Fields
		PingPong velocity;
		PingPong density;
		PingPong divergence;
		PingPong vorticity;
		PingPong pressure;

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
		ofShader gravityProgram;
	};
}