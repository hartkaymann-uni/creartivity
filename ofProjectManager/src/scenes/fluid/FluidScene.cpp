#include "FluidScene.h"

using namespace fluid;

FluidScene::FluidScene() : ccScene( "Fluid" ) {}

void FluidScene::setup()
{
	ccSolver::Settings solverSettings;
	solverSettings.timestep = 1.0f;
	solverSettings.splatRadius = 0.25f;
	solverSettings.applyVorticity = true;
	solverSettings.applyViscosity = false;
	solverSettings.viscosity = 0.3f;
	solverSettings.epsilon = 0.00024414f;
	solverSettings.curl = 0.2f;
	solverSettings.jacobiIterations = 20;
	solverSettings.dissipation = 0.998f;
	solverSettings.applyGravity = false;
	solverSettings.gravityDir = { 0.0, 1.0 };
	solverSettings.gravityStr = 9.81f;
	solver = ccSolver( solverSettings );

	ccSolver::Grid solverGrid;
	solverGrid.size = glm::vec2( 512, 256 );
	solverGrid.scale = 1.0f;
	solverGrid.applyBounds = true;
	solver.setup( solverGrid );

	// Initialize GUI parameters
	groupGeneral.setName( "General" );
	groupSolver.setName( "Solver" );
	groupBounds.setName( "Bounds" );
	groupVorticity.setName( "Vorticity" );
	groupViscosity.setName( "Viscosity" );
	groupGravity.setName( "Gravity" );
	groupGeneral.setName( "View" );

	groupGeneral.add( p_Timestep.set( "Timestep", solverSettings.timestep, 0.f, 2.f ) );
	groupGeneral.add( p_SplatRadius.set( "Splat", solverSettings.splatRadius, 0.f, 1.f ) );
	groupGeneral.add( p_Dissipation.set( "Dissipation", solverSettings.dissipation, 0.9f, 1.f ) );
	groupSolver.add( p_JacobiIterations.set( "Iterations", solverSettings.jacobiIterations, 0, 50 ) );
	groupBounds.add( p_Bounds.set( "Bounds", solverGrid.applyBounds ) );
	groupVorticity.add( p_ApplyVorticity.set( "Apply Vorticity", solverSettings.applyVorticity ) );
	groupVorticity.add( p_Epsilon.set( "Epsilon", solverSettings.epsilon, 0.f, .1f ) );
	groupVorticity.add( p_Curl.set( "Curl", solverSettings.curl, 0.f, 1.f ) );
	groupViscosity.add( p_ApplyViscosity.set( "Apply Viscosity", solverSettings.applyViscosity ) );
	groupViscosity.add( p_Viscosity.set( "Viscosity", solverSettings.viscosity, 0.f, 1.f ) );
	groupGravity.add( p_ApplyGravity.set( solverSettings.applyGravity ) );
	groupGravity.add( p_GravityDirection.set( "Direction", solverSettings.gravityDir, { -1.f, -1.f }, { 1.f, 1.f } ) );
	groupGravity.add( p_GravityStrength.set( "Strength", solverSettings.gravityStr, 0.f, 20.f ) );
	groupView.add( p_DebugView.set( "Debug", false ) );

	p_Curl.addListener( this, &FluidScene::handleCurlChanged );
	p_Bounds.addListener( this, &FluidScene::handleBoundsChanged );
	p_Epsilon.addListener( this, &FluidScene::handleEpsilonChanged );
	p_Timestep.addListener( this, &FluidScene::handleTimestepChanged );
	p_Viscosity.addListener( this, &FluidScene::handleViscosityChanged );
	p_SplatRadius.addListener( this, &FluidScene::handleSplatRadiusChanged );
	p_Dissipation.addListener( this, &FluidScene::handleDissipationChanged );
	p_ApplyGravity.addListener( this, &FluidScene::handleApplyGravityChanged );
	p_ApplyVorticity.addListener( this, &FluidScene::handleApplyVorticityChanged );
	p_ApplyViscosity.addListener( this, &FluidScene::handleApplyViscosityChanged );
	p_GravityStrength.addListener( this, &FluidScene::handleGravityStrengthChanged );
	p_JacobiIterations.addListener( this, &FluidScene::handleJacobiIterationsChanged );
	p_GravityDirection.addListener( this, &FluidScene::handleGravityDirectionChanged );

	gui.add( groupGeneral );
	gui.add( groupSolver );
	gui.add( groupBounds );
	gui.add( groupVorticity );
	gui.add( groupViscosity );
	gui.add( groupGravity );
	gui.add( groupView );

	filesystem::path shaderPath = getShaderPath();
	bool err_disp = displayVectorProgram.load( shaderPath / "passthru.vert", shaderPath / "displayvector.frag" );
}

void FluidScene::update()
{
	if (debug && !step)
		return;

	ccUser* u = userManager->getMouseUser();

	// Add user force here
	solver.step( *u );

	if (debug) step = false;
}

void FluidScene::draw()
{
	ofBackground( 0 );

	if (p_DebugView.get()) {
		//Debug view showing all fields
		ccSolver::Grid grid = *(solver.getGrid());

		ofDrawBitmapString( "density", 0.f, 0.f + 10.f );
		solver.getDensity()->draw( 0.f, 0.f );

		ofDrawBitmapString( "velocity", 0.f, grid.size.y + 10.f );
		displayVectorProgram.begin();
		displayVectorProgram.setUniformTexture( "read", solver.getVelocity()->getTexture(), 1 );
		displayVectorProgram.setUniform3f( "bias", glm::vec3( 0.5, 0.5, 0.5 ) );
		displayVectorProgram.setUniform3f( "scale", glm::vec3( 0.5, 0.5, 0.5 ) );
		displayVectorProgram.setUniform2f( "gridSize", solver.getGrid()->size );
		solver.getVelocity()->draw( 0.f, grid.size.y );

		displayVectorProgram.end();

		ofDrawBitmapString( "divergence", grid.size.x, 0.f + 10.f );
		solver.getDivergence()->draw( grid.size.x, 0.f );

		ofDrawBitmapString( "vorticity", grid.size.x, grid.size.y + 10.f );
		solver.getVorticity()->draw( grid.size.x, grid.size.y );

		ofDrawBitmapString( "pressure", 0.f, grid.size.y * 2.f + 10.f );
		solver.getPressure()->draw( 0.f, grid.size.y * 2.f );

	}
	else {
		solver.getDensity()->draw( 0.f, 0.f, width, height );
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