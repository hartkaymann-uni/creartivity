#include "FluidScene.h"

FluidScene::FluidScene() : ccScene( "Fluid" ) {}

void FluidScene::setup()
{
	plane.set( grid.size.x - 2.f, grid.size.y - 2.f );
	plane.setPosition( grid.size.x * .5f, grid.size.y * .5f, 0.f );
	plane.setResolution( 2, 2 );

	// Create render targets
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

		velocity.allocate( grid.size.x, grid.size.y, GL_RGB32F );
		velocity.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
		velocity.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
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

		density.allocate( grid.size.x, grid.size.y, GL_RGB32F );
		density.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
		density.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
		density.getTexture().loadData( cells.data(), grid.size.x, grid.size.y, GL_RGB );
	}

	velocityDivergence.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	velocityDivergence.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	velocityDivergence.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	velocityVorticity.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	velocityVorticity.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	velocityVorticity.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	pressure.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	pressure.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	pressure.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	// Create shader programs
	filesystem::path shaderPath = getShaderPath();

	bool err_adv = advectProgram.load( shaderPath / "passthru.vert", shaderPath / "advect.frag" );
	bool err_grad = gradientProgram.load( shaderPath / "passthru.vert", shaderPath / "gradient.frag" );
	bool err_jacs = jacobiscalarProgram.load( shaderPath / "passthru.vert", shaderPath / "jacobiscalar.frag" );
	bool err_jacv = jacobivectorProgram.load( shaderPath / "passthru.vert", shaderPath / "jacobivector.frag" );
	bool err_div = divergenceProgram.load( shaderPath / "passthru.vert", shaderPath / "divergence.frag" );
	bool err_vor = vorticityProgram.load( shaderPath / "passthru.vert", shaderPath / "vorticity.frag" );
	bool err_vorf = vorticityforceProgram.load( shaderPath / "passthru.vert", shaderPath / "vorticityforce.frag" );
	bool err_bounds = boundariesProgram.load( shaderPath / "passthru.vert", shaderPath / "boundaries.frag" );
	bool err_for = splatProgram.load( shaderPath / "passthru.vert", shaderPath / "splat.frag" );

	bool err_disp = displayVectorProgram.load( shaderPath / "passthru.vert", shaderPath / "displayvector.frag" );

	// Initialize GUI parameters
	groupGeneral.setName( "General" );
	groupGeneral.setName( "View" );
	groupSolver.setName( "Solver" );
	groupBounds.setName( "Bounds" );
	groupVorticity.setName( "Vorticity" );
	groupViscosity.setName( "Viscosity" );

	groupGeneral.add( p_Timestep.set( "Timestep", timestep, 0.f, 2.f ) );
	groupGeneral.add( p_SplatRadius.set( "Splat", splatRadius, 0.f, 1.f ) );
	groupGeneral.add( p_Dissipation.set( "Dissipation", dissipation, 0.9f, 1.f ) );
	groupView.add( p_DebugView.set( "Debug", false ) );
	groupBounds.add( p_Bounds.set( "Bounds", grid.applyBounds ) );
	groupSolver.add( p_JacobiIterations.set( "Iterations", jacobiIterations, 0, 50 ) );
	groupVorticity.add( p_ApplyVorticity.set( "Apply Vorticity", applyVorticity ) );
	groupVorticity.add( p_Epsilon.set( "Epsilon", epsilon, 0.f, .1f ) );
	groupVorticity.add( p_Curl.set( "Curl", curl, 0.f, 1.f ) );
	groupViscosity.add( p_ApplyViscosity.set( "Apply Viscosity", applyViscosity ) );
	groupViscosity.add( p_Viscosity.set( "Viscosity", viscosity, 0.f, 1.f ) );

	gui.add( groupGeneral );
	gui.add( groupView );
	gui.add( groupSolver );
	gui.add( groupBounds );
	gui.add( groupVorticity );
	gui.add( groupViscosity );

	gui.setPosition( width - gui.getWidth() - 10, height - gui.getHeight() - 10 );

}

void FluidScene::update()
{
	// Timer
	dt = (clock() - time) / CLOCKS_PER_SEC;
	time = clock();

	if (debug && !step)
		return;

	// Dissipation only affects density carried by the velocity field
	advect( velocity, velocity, 1 );
	boundary( velocity, velocity, -1.f );

	advect( density, density, p_Dissipation.get() );

	// Add external forces
	addForces();

	if (p_ApplyVorticity.get()) {
		vortex( velocityVorticity );
		vortexConfine( velocityVorticity, velocity );
		boundary( velocity, velocity, -1.f );
	}

	if (p_ApplyViscosity.get() && p_Viscosity.get() > 0.f) {
		float s = grid.scale;

		float alpha = (s * s) / (p_Viscosity.get() * p_Timestep.get());
		float beta = 4.f + alpha;

		diffuse( jacobivectorProgram, velocity, velocity, velocity, alpha, beta, -1.f );
	}

	project();

	if (debug) step = false;
}

void FluidScene::project() {
	diverge( velocityDivergence );

	// Start of with 0 for poisson equation
	clearBuffer( pressure );

	float alpha = -grid.scale * grid.scale;
	diffuse( jacobiscalarProgram, pressure, velocityDivergence, pressure, alpha, 4.f, 1.f );
	gradiate( velocity );

	boundary( velocity, velocity, -1.f );
}

void FluidScene::addForces() {
	// Mouse Unteraction, can be extended for actual interaction with a simple for loop
	ccUser* u = userManager->getMouseUser();
	glm::vec3 color = u->getMotion();

	if (color.x != 0.f || color.y != 0) {
		glm::vec3 pos = u->getPositon();
		float xMapped = ofMap( pos.x, 0, width, 0, grid.size.x );
		float yMapped = ofMap( pos.y, 0, height, 0, grid.size.y );

		glm::vec2 point = { xMapped , yMapped };

		splat( velocity, color, point );
		splat( density, { 1.0, 1.0, 1.0 }, point );
	}

}

void FluidScene::advect( ofFbo& advected, ofFbo& output, float d ) {
	advectProgram.begin();

	advectProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	advectProgram.setUniformTexture( "advected", advected.getTexture(), 1 );
	advectProgram.setUniform2f( "gridSize", grid.size );
	advectProgram.setUniform1f( "gridScale", grid.scale );
	advectProgram.setUniform1f( "timestep", p_Timestep.get() );
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

void FluidScene::boundary( ofFbo& input, ofFbo& output, float scale ) {
	if (!p_Bounds.get())
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

void FluidScene::boundarySide( ofFbo& input, ofFbo& output, ofPolyline& line, glm::vec2 offset, float scale ) {
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


void FluidScene::vortex( ofFbo& output ) {
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

void FluidScene::vortexConfine( ofFbo& vorticity, ofFbo& output ) {
	vorticityforceProgram.begin();

	vorticityforceProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	vorticityforceProgram.setUniformTexture( "vorticity", vorticity.getTexture(), 1 );
	vorticityforceProgram.setUniform2f( "gridSize", grid.size );
	vorticityforceProgram.setUniform1f( "gridScale", grid.scale );
	vorticityforceProgram.setUniform1f( "timestep", p_Timestep.get() );
	vorticityforceProgram.setUniform1f( "epsilon", p_Epsilon.get() );
	vorticityforceProgram.setUniform2f( "curl", p_Curl.get() * grid.scale, p_Curl.get() * grid.scale );

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

void FluidScene::diffuse( ofShader& jacobi, ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta, float scale ) {
	for (int i = 0; i < p_JacobiIterations.get(); i++) {
		diffuseStep( jacobi, x, b, output, alpha, beta );
	}
}

void FluidScene::diffuseStep( ofShader& jacobi, ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta ) {
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

void FluidScene::diverge( ofFbo& divergence ) {
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

void FluidScene::gradiate( ofFbo& output ) {
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

void FluidScene::splat( ofFbo& read, glm::vec3 color, glm::vec2 point ) {
	splatProgram.begin();

	splatProgram.setUniformTexture( "read", read, 2 );
	splatProgram.setUniform2f( "gridSize", grid.size );
	splatProgram.setUniform3f( "color", color );
	splatProgram.setUniform2f( "point", point );
	splatProgram.setUniform1f( "radius", p_SplatRadius.get() );

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

void FluidScene::clearBuffer( ofFbo& buffer ) {
	buffer.begin();
	ofClear( 0 );
	buffer.end();
}

void FluidScene::draw()
{
	ofBackground( 0 );

	if (p_DebugView.get()) {
		//Debug view showing all fields

		ofDrawBitmapString( "density", 0.f, 0.f + 10.f );
		density.draw( 0.f, 0.f );

		ofDrawBitmapString( "velocity", 0.f, grid.size.y + 10.f );
		displayVectorProgram.begin();
		displayVectorProgram.setUniformTexture( "read", velocity.getTexture(), 1 );
		displayVectorProgram.setUniform3f( "bias", glm::vec3( 0.5, 0.5, 0.5 ) );
		displayVectorProgram.setUniform3f( "scale", glm::vec3( 0.5, 0.5, 0.5 ) );
		displayVectorProgram.setUniform2f( "gridSize", grid.size );
		velocity.draw( 0.f, grid.size.y );

		displayVectorProgram.end();

		ofDrawBitmapString( "divergence", grid.size.x, 0.f + 10.f );
		velocityDivergence.draw( grid.size.x, 0.f );

		ofDrawBitmapString( "vorticity", grid.size.x, grid.size.y + 10.f );
		velocityVorticity.draw( grid.size.x, grid.size.y );

		ofDrawBitmapString( "pressure", 0.f, grid.size.y * 2.f + 10.f );
		pressure.draw( 0.f, grid.size.y * 2.f );
	}
	else {
		density.draw( 0.f, 0.f, width, height );
	}
}

void FluidScene::keyPressed( int key ) {

	//std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
	}
	else if (key == 'r' || key == 'R') {
		resetCamera();
	}
	else if (key == 'd' || key == 'D') {
		debug = !debug;
		cout << "Debug: " << (debug ? "On" : "Off") << endl;
	}
	else if (key == 32) {
		if (debug) step = true;
	}
}

void FluidScene::keyReleased( int key ) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();
	}
}
