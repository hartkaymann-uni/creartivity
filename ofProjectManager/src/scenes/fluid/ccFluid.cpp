#include "ccFluid.h"

namespace fluid {

	ccSolver::ccSolver() {}

	ccSolver::ccSolver( Settings& settings )
		: s( settings ) {}

	void ccSolver::setup( Grid& grid )
	{
		this->grid = grid;

		plane.set( grid.size.x - 2.f, grid.size.y - 2.f );
		plane.setPosition( grid.size.x * .5f, grid.size.y * .5f, 0.f );
		plane.setResolution( 2, 2 );

		// Create render targets
		ofDisableArbTex();
		velocity.allocate( grid.size, GL_RGB16_SNORM );
		density.allocate( grid.size, GL_RGB16_SNORM );
		divergence.allocate( grid.size, GL_RGB16_SNORM );
		vorticity.allocate( grid.size, GL_RGB16_SNORM );
		pressure.allocate( grid.size, GL_RGB16_SNORM );
		ofEnableArbTex();

		// Create shader programs
		filesystem::path shaderPath = "../../src/scenes/fluid/shader";

		bool err_adv = advectProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/advect.frag" );
		bool err_for = splatProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/splat.frag" );
		bool err_grad = gradientProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/gradient.frag" );
		bool err_vor = vorticityProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/vorticity.frag" );
		bool err_div = divergenceProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/divergence.frag" );
		bool err_bounds = boundariesProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/boundaries.frag" );
		bool err_jacv = jacobivectorProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/jacobivector.frag" );
		bool err_jacs = jacobiscalarProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/jacobiscalar.frag" );
		bool err_vorf = vorticityforceProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/vorticityforce.frag" );
		bool err_grav = gravityProgram.load( shaderPath / "passthru.vert", shaderPath / "fluid/gravity.frag" );
	}

	void ccSolver::colorDensity(ofShader& shader)
	{
		shader.begin();
		density.write->begin();
		ofClear( 0 );
		plane.draw();
		density.write->end();
		density.swap();
		shader.end();
	}

	ofFbo ccSolver::createFbo( int format ) {
		ofFbo temp;
		temp.allocate( grid.size.x, grid.size.y, format );
		temp.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
		temp.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
		return temp;
	}

	void ccSolver::step( vector<ccUser> users )
	{
		if ( s.applyGravity ) {
			gravitate( velocity );
		}

		// Dissipation only affects density carried by the velocity field
		advect( velocity, velocity, 1 );
		boundary( velocity, velocity, -1.f );

		advect( density, density, s.dissipation );

		// Add external forces
		for ( vector<ccUser>::iterator it = users.begin(); it != users.end(); it++ ) {
			addForces( *it );
		}

		if ( s.applyVorticity ) {
			vortex( vorticity );
			vortexConfine( vorticity, velocity );
			boundary( velocity, velocity, -1.f );
		}
		if ( s.applyViscosity && s.viscosity > 0.f ) {
			float scale = grid.scale;

			float alpha = (scale * scale) / (s.viscosity * s.timestep);
			float beta = 4.f + alpha;

			diffuse( jacobivectorProgram, velocity, velocity, velocity, alpha, beta, -1.f );
		}

		project();
	}

	void ccSolver::reset()
	{
		velocity.clear();
		density.clear();
		divergence.clear();
		vorticity.clear();
		pressure.clear();
	}

	void ccSolver::project() {
		diverge( divergence );

		// Start of with 0 for poisson equation
		pressure.clear();
		pressure.write->begin();
		ofClear( 0 );
		pressure.write->end();

		float alpha = -grid.scale * grid.scale;
		diffuse( jacobiscalarProgram, pressure, divergence, pressure, alpha, 4.f, 1.f );
		gradiate( velocity );

		boundary( pressure, pressure, 1.f );
		boundary( velocity, velocity, -1.f );
	}

	void ccSolver::addForces( ccUser& user ) {
		glm::vec3 colorL = user.getMotions().first;
		glm::vec3 colorR = user.getMotions().second;

		// Left
		if ( colorL.x != 0.f || colorL.y != 0 ) {
			glm::vec2 point = user.left();
			// Map positions to grid
			ofFloatColor c = s.splatColor;
			splat( velocity, glm::normalize( colorL ), point );
			if ( s.splatDensity ) splat( density, { c.r, c.g, c.b }, point );
		}
		// Right
		if ( colorR.x != 0.f || colorR.y != 0 ) {
			glm::vec2 point = user.right();
			// Map positions to grid
			ofFloatColor c = s.splatColor;
			splat( velocity, glm::normalize( colorR ), point );
			if ( s.splatDensity )splat( density, { c.r, c.g, c.b }, point );
		}
		boundary( velocity, velocity, -1.f );
	}

	void ccSolver::advect( PingPong& advected, PingPong& output, float d ) {
		advectProgram.begin();

		advectProgram.setUniformTexture( "velocity", velocity.read->getTexture(), 2 );
		advectProgram.setUniformTexture( "advected", advected.read->getTexture(), 1 );
		advectProgram.setUniform2f( "gridSize", grid.size );
		advectProgram.setUniform1f( "gridScale", grid.scale );
		advectProgram.setUniform1f( "timestep", s.timestep );
		advectProgram.setUniform1f( "dissipation", d );

		output.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		output.write->end();
		output.swap();

		advectProgram.end();
	}

	// Computes the boundaries of the simulation domain
	void ccSolver::boundary( PingPong& input, PingPong& output, float scale ) {
		if ( !grid.applyBounds )
			return;

		// default offset: 1
		float offset = 1.f;
		float xL = offset;
		float xR = grid.size.x;
		float yB = offset;
		float yT = grid.size.y;

		ofPolyline lineR, lineL, lineT, lineB;
		lineL.addVertex( xL, yT );
		lineL.addVertex( xL, yB );
		lineR.addVertex( xR, yB );
		lineR.addVertex( xR, yT );
		lineB.addVertex( xR, yB );
		lineB.addVertex( xL, yB );
		lineT.addVertex( xR, yT );
		lineT.addVertex( xL, yT );

		boundarySide( input, output, lineL, { 1.f, 0.f }, scale );
		boundarySide( input, output, lineR, { -1.f, 0.f }, scale );
		boundarySide( input, output, lineB, { 0.f, 1.f }, scale );
		boundarySide( input, output, lineT, { 0.f, -1.f }, scale );
		// no swapping here, will be done by the next slabob
	}

	// Apply boundary logic to one side
	void ccSolver::boundarySide( PingPong& input, PingPong& output, ofPolyline& line, glm::vec2 offset, float scale ) {
		boundariesProgram.begin();

		boundariesProgram.setUniformTexture( "read", input.read->getTexture(), 1 );
		boundariesProgram.setUniform2f( "gridSize", grid.size );
		boundariesProgram.setUniform2f( "gridOffset", offset );
		boundariesProgram.setUniform1f( "scale", scale );

		output.read->begin();
		// Draw one line to apply bounds to one side
		line.draw();
		output.read->end();

		boundariesProgram.end();
	}

	void ccSolver::vortex( PingPong& output ) {
		vorticityProgram.begin();

		vorticityProgram.setUniformTexture( "velocity", velocity.read->getTexture(), 2 );
		vorticityProgram.setUniform2f( "gridSize", grid.size );
		vorticityProgram.setUniform1f( "gridScale", grid.scale );

		output.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		output.write->end();
		output.swap();

		vorticityProgram.end();
	}

	void ccSolver::vortexConfine( PingPong& vorticity, PingPong& output ) {
		vorticityforceProgram.begin();

		vorticityforceProgram.setUniformTexture( "velocity", velocity.read->getTexture(), 2 );
		vorticityforceProgram.setUniformTexture( "vorticity", vorticity.read->getTexture(), 1 );
		vorticityforceProgram.setUniform2f( "gridSize", grid.size );
		vorticityforceProgram.setUniform1f( "gridScale", grid.scale );
		vorticityforceProgram.setUniform1f( "timestep", s.timestep );
		vorticityforceProgram.setUniform1f( "epsilon", s.epsilon );
		vorticityforceProgram.setUniform2f( "curl", s.curl * grid.scale, s.curl * grid.scale );

		output.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		output.write->end();
		output.swap();

		vorticityforceProgram.end();
	}

	void ccSolver::diffuse( ofShader& jacobi, PingPong& x, PingPong& b, PingPong& output, float alpha, float beta, float scale ) {
		for ( int i = 0; i < s.jacobiIterations; i++ ) {
			diffuseStep( jacobi, x, b, output, alpha, beta );
			boundary( output, output, scale );
		}

	}

	void ccSolver::diffuseStep( ofShader& jacobi, PingPong& x, PingPong& b, PingPong& output, float alpha, float beta ) {
		jacobi.begin();

		jacobi.setUniformTexture( "x", x.read->getTexture(), 1 );
		jacobi.setUniformTexture( "b", b.read->getTexture(), 2 );
		jacobi.setUniform2f( "gridSize", grid.size );
		jacobi.setUniform1f( "gridScale", grid.scale );
		jacobi.setUniform1f( "alpha", alpha );
		jacobi.setUniform1f( "beta", beta );

		output.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		output.write->end();
		output.swap();

		jacobi.end();
	}

	void ccSolver::diverge( PingPong& divergence ) {
		divergenceProgram.begin();

		divergenceProgram.setUniformTexture( "velocity", velocity.read->getTexture(), 2 );
		divergenceProgram.setUniform2f( "gridSize", grid.size );
		divergenceProgram.setUniform1f( "gridScale", grid.scale );

		divergence.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		divergence.write->end();
		divergence.swap();

		divergenceProgram.end();
	}

	void ccSolver::gradiate( PingPong& output ) {
		gradientProgram.begin();

		gradientProgram.setUniformTexture( "p", pressure.read->getTexture(), 1 );
		gradientProgram.setUniformTexture( "w", velocity.read->getTexture(), 2 );
		gradientProgram.setUniform2f( "gridSize", grid.size );
		gradientProgram.setUniform1f( "gridScale", grid.scale );

		output.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		output.write->end();
		output.swap();

		gradientProgram.end();
	}

	void ccSolver::splat( PingPong& read, glm::vec3 color, glm::vec2 point ) {
		splatProgram.begin();

		splatProgram.setUniformTexture( "read", read.read->getTexture(), 2 );
		splatProgram.setUniform2f( "gridSize", grid.size );
		splatProgram.setUniform3f( "color", color );
		splatProgram.setUniform2f( "point", point );
		splatProgram.setUniform1f( "radius", s.splatRadius );

		read.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		read.write->end();
		read.swap();

		splatProgram.end();
	}

	void ccSolver::gravitate( PingPong& output ) {
		gravityProgram.begin();

		gravityProgram.setUniformTexture( "velocity", velocity.read->getTexture(), 2 );
		gravityProgram.setUniform2f( "dir", s.gravityDir );
		gravityProgram.setUniform1f( "str", s.gravityStr );

		output.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		output.write->end();
		output.swap();

		gravityProgram.end();
	}

}