#pragma once

#include "ofMain.h"
#include "ccUser.h"

/*
	Fluid Solver
	Simulates two-dimensional fluid based on an eulerian (grid-based) approach to solving the Navier-Stokes Equations
	Logic is done in fragment shaders and applied to vector or scalar fields represented by textures (Field struct)
	Vorticity and viscosity can be applied as well
*/

namespace fluid {

	/*
		Field structure used by solver
		Ping pong buffer for representing vector / scalar fields
	*/
	struct Field {
	public:
		void allocate( glm::vec2 dim, int internalFormat = GL_RGBA ) {
			// Allocate
			for (int i = 0; i < 2; i++) {
				FBOs[i].allocate( dim.x, dim.y, internalFormat );
				FBOs[i].getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
				FBOs[i].getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
			}

			//Assign
			read = &FBOs[0];
			write = &FBOs[1];

			// Clean
			clear();
		}

		void swap() {
			std::swap( read, write );
		}

		void clear() {
				FBOs[0].begin();
				ofClear( 0 );
				FBOs[0].end();
				swap();
		}

		ofFbo& operator[]( int n ) { return FBOs[n]; }
		ofFbo* read;
		ofFbo* write;

	private:
		ofFbo   FBOs[2];    // Real addresses of FBOs
	};


	class ccSolver
	{
	public:
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
		};

		struct Grid {
			glm::vec2 size;
			float scale = 1.f;
			bool applyBounds;
		};

		ccSolver();
		ccSolver( Settings& settings );

		void setup( Grid& grid );
		void step( vector<ccUser> users );

		inline Grid* const getGrid() { return &grid; }
		inline ofFbo* const getDensity() { return density.read; }
		inline ofFbo* const getVelocity() { return velocity.read; }
		inline ofFbo* const getDivergence() { return divergence.read; }
		inline ofFbo* const getVorticity() { return vorticity.read; }
		inline ofFbo* const getPressure() { return pressure.read; }

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

	private:
		ofFbo createFbo( int format );

		void project();
		void addForces( ccUser& user );

		// Slabops
		void advect( Field& advected, Field& output, float d );
		void boundary( Field& input, Field& output, float scale = -1.f );
		void boundarySide( Field& input, Field& output, ofPolyline& line, glm::vec2 offset, float scale );
		void vortex( Field& output );
		void vortexConfine( Field& vorticity, Field& output );
		void diffuse( ofShader& jacobi, Field& x, Field& b, Field& output, float alpha = -1.f, float beta = 4.f, float scale = -1.f );
		void diffuseStep( ofShader& jacobi, Field& x, Field& b, Field& output, float alpha, float beta );
		void diverge( Field& divergence );
		void gradiate( Field& output );
		void splat( Field& read, glm::vec3 color, glm::vec2 point );
		void gravitate( Field& output );

		Settings s;
		Grid grid;

		ofPlanePrimitive plane;

		// Vector Fields
		Field velocity;
		Field density;
		Field divergence;
		Field vorticity;
		Field pressure;

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