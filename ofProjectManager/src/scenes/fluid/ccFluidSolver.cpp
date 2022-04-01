#include "ccFluidSolver.h"

ccFluidSolver::ccFluidSolver() {}

ccFluidSolver::ccFluidSolver( Settings& settings )
	: s( settings ) {}

void ccFluidSolver::setup( Grid& grid )
{
	this->grid = grid;

	plane.set( grid.size.x - 2.f, grid.size.y - 2.f );
	plane.setPosition( grid.size.x * .5f, grid.size.y * .5f, 0.f );
	plane.setResolution( 2, 2 );

	// Create render targets
	velocity = createFbo();
	density = createFbo();
	divergence = createFbo();
	vorticity = createFbo();
	pressure = createFbo();

	{
		int no_pixels = grid.size.x * grid.size.y * 3;
		vector<float> cells( no_pixels );
		for (size_t x = 0; x < grid.size.y; x++) {
			for (size_t y = 0; y < grid.size.x; y++) {
				size_t i = x * grid.size.x + y;

				cells[i * 3 + 0] = ofMap( ofNoise( 2.f * x / grid.size.y, 2.f * y / grid.size.x ), 0.0, 1.0, -1.0, 1.0 );
				cells[i * 3 + 1] = ofMap( ofNoise( x / grid.size.x, y / grid.size.y ), 0.0, 1.0, -1.0, 1.0 );
				cells[i * 3 + 2] = 0.0;
			}
		}
		velocity.getTexture().loadData( cells.data(), grid.size.x, grid.size.y, GL_RGB );
	}

	{
		int no_pixels = grid.size.x * grid.size.y * 3;
		vector<float> cells( no_pixels );
		for (size_t x = 0; x < grid.size.y; x++) {
			for (size_t y = 0; y < grid.size.x; y++) {
				size_t i = x * grid.size.x + y;
				float initialValue = ofNoise( 2.f * x / grid.size.y, 2.f * y / grid.size.x );

				cells[i * 3 + 0] = initialValue;
				cells[i * 3 + 1] = initialValue;
				cells[i * 3 + 2] = initialValue;
			}
		}
		density.getTexture().loadData( cells.data(), grid.size.x, grid.size.y, GL_RGB );
	}

	// Create shader programs
	filesystem::path shaderPath = "../../src/scenes/fluid/shader";

	bool err_adv = advectProgram.load( shaderPath / "passthru.vert", shaderPath / "advect.frag" );
	bool err_for = splatProgram.load( shaderPath / "passthru.vert", shaderPath / "splat.frag" );
	bool err_grad = gradientProgram.load( shaderPath / "passthru.vert", shaderPath / "gradient.frag" );
	bool err_vor = vorticityProgram.load( shaderPath / "passthru.vert", shaderPath / "vorticity.frag" );
	bool err_div = divergenceProgram.load( shaderPath / "passthru.vert", shaderPath / "divergence.frag" );
	bool err_bounds = boundariesProgram.load( shaderPath / "passthru.vert", shaderPath / "boundaries.frag" );
	bool err_jacv = jacobivectorProgram.load( shaderPath / "passthru.vert", shaderPath / "jacobivector.frag" );
	bool err_jacs = jacobiscalarProgram.load( shaderPath / "passthru.vert", shaderPath / "jacobiscalar.frag" );
	bool err_vorf = vorticityforceProgram.load( shaderPath / "passthru.vert", shaderPath / "vorticityforce.frag" );
}

ofFbo ccFluidSolver::createFbo() {
	ofFbo temp;
	temp.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	temp.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	temp.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
	return temp;
}

void ccFluidSolver::step( ccUser& user )
{	// Dissipation only affects density carried by the velocity field
	advect( velocity, velocity, 1 );
	boundary( velocity, velocity, -1.f );

	advect( density, density, s.dissipation );

	// Add external forces
	addForces( user );

	if (s.applyVorticity) {
		vortex( vorticity );
		vortexConfine( vorticity, velocity );
		boundary( velocity, velocity, -1.f );
	}

	if (s.applyViscosity && s.viscosity > 0.f) {
		float scale = grid.scale;

		float alpha = (scale * scale) / (s.viscosity * s.timestep);
		float beta = 4.f + alpha;

		diffuse( jacobivectorProgram, velocity, velocity, velocity, alpha, beta, -1.f );
	}

	project();
}

void ccFluidSolver::project() {
	diverge( divergence );

	// Start of with 0 for poisson equation
	clearBuffer( pressure );

	float alpha = -grid.scale * grid.scale;
	diffuse( jacobiscalarProgram, pressure, divergence, pressure, alpha, 4.f, 1.f );
	gradiate( velocity );

	boundary( velocity, velocity, -1.f );
}

void ccFluidSolver::addForces( ccUser& user ) {
	// Mouse Unteraction, can be extended for actual interaction with a simple for loop
	glm::vec3 color = user.getMotion();

	if (color.x != 0.f || color.y != 0) {
		glm::vec3 pos = user.getPositon();
		float xMapped = ofMap( pos.x, 0, ofGetWidth(), 0, grid.size.x );
		float yMapped = ofMap( pos.y, 0, ofGetHeight(), 0, grid.size.y );

		glm::vec2 point = { xMapped , yMapped };

		splat( velocity, color, point );
		splat( density, { 1.0, 1.0, 1.0 }, point );
	}

}

void ccFluidSolver::advect( ofFbo& advected, ofFbo& output, float d ) {
	advectProgram.begin();

	advectProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	advectProgram.setUniformTexture( "advected", advected.getTexture(), 1 );
	advectProgram.setUniform2f( "gridSize", grid.size );
	advectProgram.setUniform1f( "gridScale", grid.scale );
	advectProgram.setUniform1f( "timestep", s.timestep );
	advectProgram.setUniform1f( "dissipation", d );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	plane.draw();
	outBuffer.end();

	output = outBuffer;

	advectProgram.end();
}

void ccFluidSolver::boundary( ofFbo& input, ofFbo& output, float scale ) {
	if (!grid.applyBounds)
		return;

	float xL = 0.f;
	float xR = grid.size.x - 0.f;
	float yT = 0.f;
	float yB = grid.size.y - 0.f;

	ofPolyline lineR, lineL, lineT, lineB;
	lineR.addVertex( xR, yB );
	lineR.addVertex( xR, yT );
	lineL.addVertex( xL, yB );
	lineL.addVertex( xL, yT );
	lineT.addVertex( xL, yT );
	lineT.addVertex( xR, yT );
	lineB.addVertex( xL, yB );
	lineB.addVertex( xR, yB );

	boundarySide( input, output, lineR, { -1.f, 0.f }, scale );
	boundarySide( input, output, lineL, { 1.f, 0.f }, scale );
	boundarySide( input, output, lineT, { 0.f, -1.f }, scale );
	boundarySide( input, output, lineB, { 0.f, 1.f }, scale );
}

void ccFluidSolver::boundarySide( ofFbo& input, ofFbo& output, ofPolyline& line, glm::vec2 offset, float scale ) {
	boundariesProgram.begin();

	boundariesProgram.setUniformTexture( "read", input.getTexture(), 0 );
	boundariesProgram.setUniform2f( "gridSize", grid.size );
	boundariesProgram.setUniform2f( "gridOffset", offset );
	boundariesProgram.setUniform1f( "scale", scale );

	output.begin();

	// Draw one line to apply bounds to one side
	line.draw();

	output.end();
	boundariesProgram.end();
}


void ccFluidSolver::vortex( ofFbo& output ) {
	vorticityProgram.begin();

	vorticityProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	vorticityProgram.setUniform2f( "gridSize", grid.size );
	vorticityProgram.setUniform1f( "gridScale", grid.scale );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	plane.draw();
	outBuffer.end();

	output = outBuffer;

	vorticityProgram.end();
}

void ccFluidSolver::vortexConfine( ofFbo& vorticity, ofFbo& output ) {
	vorticityforceProgram.begin();

	vorticityforceProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	vorticityforceProgram.setUniformTexture( "vorticity", vorticity.getTexture(), 1 );
	vorticityforceProgram.setUniform2f( "gridSize", grid.size );
	vorticityforceProgram.setUniform1f( "gridScale", grid.scale );
	vorticityforceProgram.setUniform1f( "timestep", s.timestep );
	vorticityforceProgram.setUniform1f( "epsilon", s.epsilon );
	vorticityforceProgram.setUniform2f( "curl", s.curl * grid.scale, s.curl * grid.scale );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	plane.draw();
	outBuffer.end();

	output = outBuffer;

	vorticityforceProgram.end();
}

void ccFluidSolver::diffuse( ofShader& jacobi, ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta, float scale ) {
	for (int i = 0; i < s.jacobiIterations; i++) {
		diffuseStep( jacobi, x, b, output, alpha, beta );
	}
}

void ccFluidSolver::diffuseStep( ofShader& jacobi, ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta ) {
	jacobi.begin();

	jacobi.setUniformTexture( "x", x.getTexture(), 2 );
	jacobi.setUniformTexture( "b", b.getTexture(), 1 );
	jacobi.setUniform2f( "gridSize", grid.size );
	jacobi.setUniform1f( "gridScale", grid.scale );
	jacobi.setUniform1f( "alpha", alpha );
	jacobi.setUniform1f( "beta", beta );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	plane.draw();
	outBuffer.end();

	output = outBuffer;

	jacobi.end();

}

void ccFluidSolver::diverge( ofFbo& divergence ) {
	divergenceProgram.begin();

	divergenceProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	divergenceProgram.setUniform2f( "gridSize", grid.size );
	divergenceProgram.setUniform1f( "gridScale", grid.scale );

	divergence.begin();
	ofClear( 0 );
	ofFill();
	plane.draw();
	divergence.end();

	divergenceProgram.end();
}

void ccFluidSolver::gradiate( ofFbo& output ) {
	gradientProgram.begin();

	gradientProgram.setUniformTexture( "p", pressure.getTexture(), 1 );
	gradientProgram.setUniformTexture( "w", velocity.getTexture(), 2 );
	gradientProgram.setUniform2f( "gridSize", grid.size );
	gradientProgram.setUniform1f( "gridScale", grid.scale );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	plane.draw();
	outBuffer.end();

	output = outBuffer;

	gradientProgram.end();
}

void ccFluidSolver::splat( ofFbo& read, glm::vec3 color, glm::vec2 point ) {
	splatProgram.begin();

	splatProgram.setUniformTexture( "read", read, 2 );
	splatProgram.setUniform2f( "gridSize", grid.size );
	splatProgram.setUniform3f( "color", color );
	splatProgram.setUniform2f( "point", point );
	splatProgram.setUniform1f( "radius", s.splatRadius );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	plane.draw();
	outBuffer.end();

	read = outBuffer;

	splatProgram.end();
}

void ccFluidSolver::clearBuffer( ofFbo& buffer ) {
	buffer.begin();
	ofClear( 0 );
	buffer.end();
}

ofFbo* ccFluidSolver::getDensity() {
	return &density;
}