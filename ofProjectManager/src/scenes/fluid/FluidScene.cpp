#include "FluidScene.h"

FluidScene::FluidScene() : ccScene( "Fluid" ) {}

void FluidScene::setup()
{
	ccFluidSolver::Settings solverSettings;
	solverSettings.timestep = 1.0f;
	solverSettings.splatRadius = 0.1f;
	solverSettings.applyVorticity = true;
	solverSettings.applyViscosity = false;
	solverSettings.viscosity = 0.3f;
	solverSettings.epsilon = 0.00024414f;
	solverSettings.curl = 0.3f;
	solverSettings.jacobiIterations = 20;
	solverSettings.dissipation = 0.998f;
	solver = ccFluidSolver( solverSettings );

	ccFluidSolver::Grid solverGrid;
	solverGrid.size = glm::vec2( 512, 256 );
	solverGrid.scale = 1.0f;
	solverGrid.applyBounds = false;
	solver.setup( solverGrid );


	// Initialize GUI parameters
	groupGeneral.setName( "General" );
	groupGeneral.setName( "View" );
	groupSolver.setName( "Solver" );
	groupBounds.setName( "Bounds" );
	groupVorticity.setName( "Vorticity" );
	groupViscosity.setName( "Viscosity" );

	groupGeneral.add( p_Timestep.set( "Timestep", solverSettings.timestep, 0.f, 2.f ) );
	groupGeneral.add( p_SplatRadius.set( "Splat", solverSettings.splatRadius, 0.f, 1.f ) );
	groupGeneral.add( p_Dissipation.set( "Dissipation", solverSettings.dissipation, 0.9f, 1.f ) );
	groupView.add( p_DebugView.set( "Debug", false ) );
	groupBounds.add( p_Bounds.set( "Bounds", solverGrid.applyBounds ) );
	groupSolver.add( p_JacobiIterations.set( "Iterations", solverSettings.jacobiIterations, 0, 50 ) );
	groupVorticity.add( p_ApplyVorticity.set( "Apply Vorticity", solverSettings.applyVorticity ) );
	groupVorticity.add( p_Epsilon.set( "Epsilon", solverSettings.epsilon, 0.f, .1f ) );
	groupVorticity.add( p_Curl.set( "Curl", solverSettings.curl, 0.f, 1.f ) );
	groupViscosity.add( p_ApplyViscosity.set( "Apply Viscosity", solverSettings.applyViscosity ) );
	groupViscosity.add( p_Viscosity.set( "Viscosity", solverSettings.viscosity, 0.f, 1.f ) );

	p_Timestep.addListener( this, &ccFluidSolver::setTimestep );

	gui.add( groupGeneral );
	gui.add( groupView );
	gui.add( groupSolver );
	gui.add( groupBounds );
	gui.add( groupVorticity );
	gui.add( groupViscosity );

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
		/*
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
		*/
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