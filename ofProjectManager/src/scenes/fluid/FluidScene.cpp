#include "FluidScene.h"

using namespace fluid;

FluidScene::FluidScene()
	: ccScene( "Fluid" ),
	time( 0.f ),
	debug( false ),
	step( false ),
	sequenceDuration( 10.f ),
	sequenceTransitionDuration( 1.f ),
	lastSequene( SequenceName::Empty ),
	currentSequence( SequenceName::Empty ),
	lastSequenceTime( 0.f ),
	shading( DEFAULT ) {}

void FluidScene::setup()
{
	// Initialize all values necessary for the solver
	ccSolver::Settings solverSettings;
	solverSettings.timestep = 1.0f;
	solverSettings.splatRadius = 0.002f;
	solverSettings.splatColor = ofFloatColor( 1.f );
	solverSettings.applyVorticity = false;
	solverSettings.applyViscosity = false;
	solverSettings.viscosity = 0.3f;
	solverSettings.epsilon = 0.00024414f;
	solverSettings.curl = 0.1f;
	solverSettings.jacobiIterations = 40;
	solverSettings.dissipation = 0.998f;
	solverSettings.applyGravity = false;
	solverSettings.gravityDir = { 0.0, 1.0 };
	solverSettings.gravityStr = 9.81f;
	solver = ccSolver( solverSettings );

	ccSolver::Grid solverGrid;
	solverGrid.size = glm::vec2( 3840, 1080); // WQHD : 2.560 x 1.440
	solverGrid.scale = 1.0f;
	solverGrid.applyBounds = true;
	solver.setup( solverGrid );

	// Initialize GUI parameters
	groupGeneral.setName( "General" );
	groupSolver.setName( "Solver" );
	groupGrid.setName( "Grid" );
	groupVorticity.setName( "Vorticity" );
	groupViscosity.setName( "Viscosity" );
	groupGravity.setName( "Gravity" );
	groupGeneral.setName( "View" );

	groupGeneral.add( p_Timestep.set( "Timestep", solverSettings.timestep, 0.f, 2.f ) );
	groupGeneral.add( p_SplatRadius.set( "Splat", solverSettings.splatRadius, 0.f, .005f ) );
	groupGeneral.add( p_SplatColor.set( "Color", solverSettings.splatColor ) );
	groupGeneral.add( p_Dissipation.set( "Dissipation", solverSettings.dissipation, 0.9f, 1.f ) );
	groupSolver.add( p_JacobiIterations.set( "Iterations", solverSettings.jacobiIterations, 0, 120 ) );
	groupGrid.add( p_Bounds.set( "Bounds", solverGrid.applyBounds ) );
	groupGrid.add( p_Scale.set( "Scale", 1.f, 0.f, 1.f ) );
	groupVorticity.add( p_ApplyVorticity.set( "Apply Vorticity", solverSettings.applyVorticity ) );
	groupVorticity.add( p_Epsilon.set( "Epsilon", solverSettings.epsilon, 0.f, .1f ) );
	groupVorticity.add( p_Curl.set( "Curl", solverSettings.curl, 0.f, 1.f ) );
	groupViscosity.add( p_ApplyViscosity.set( "Apply Viscosity", solverSettings.applyViscosity ) );
	groupViscosity.add( p_Viscosity.set( "Viscosity", solverSettings.viscosity, 0.f, 1.f ) );
	groupGravity.add( p_ApplyGravity.set( solverSettings.applyGravity ) );
	groupGravity.add( p_GravityDirection.set( "Direction", solverSettings.gravityDir, { -1.f, -1.f }, { 1.f, 1.f } ) );
	groupGravity.add( p_GravityStrength.set( "Strength", solverSettings.gravityStr, 0.f, 20.f ) );
	groupView.add( p_Sequences.set( "RunSequences", true ) );
	groupView.add( p_DebugView.set( "Debug", false ) );

	p_Curl.addListener( this, &FluidScene::handleCurlChanged );
	p_Scale.addListener( this, &FluidScene::handleScaleChanged );
	p_Bounds.addListener( this, &FluidScene::handleBoundsChanged );
	p_Epsilon.addListener( this, &FluidScene::handleEpsilonChanged );
	p_Timestep.addListener( this, &FluidScene::handleTimestepChanged );
	p_Viscosity.addListener( this, &FluidScene::handleViscosityChanged );
	p_SplatRadius.addListener( this, &FluidScene::handleSplatRadiusChanged );
	p_SplatColor.addListener( this, &FluidScene::handleSplatColorChanged );
	p_Dissipation.addListener( this, &FluidScene::handleDissipationChanged );
	p_ApplyGravity.addListener( this, &FluidScene::handleApplyGravityChanged );
	p_ApplyVorticity.addListener( this, &FluidScene::handleApplyVorticityChanged );
	p_ApplyViscosity.addListener( this, &FluidScene::handleApplyViscosityChanged );
	p_GravityStrength.addListener( this, &FluidScene::handleGravityStrengthChanged );
	p_JacobiIterations.addListener( this, &FluidScene::handleJacobiIterationsChanged );
	p_GravityDirection.addListener( this, &FluidScene::handleGravityDirectionChanged );

	// Add all gui parameter groups
	gui.add( groupGeneral );
	gui.add( groupSolver );
	gui.add( groupGrid );
	gui.add( groupVorticity );
	gui.add( groupViscosity );
	//gui.add( groupGravity );
	gui.add( groupView );

	// Load display shaders
	filesystem::path shaderPath = getShaderPath();
	bool err_loadscalar = displayScalar.load( shaderPath / "passthru.vert", shaderPath / "displayscalar.frag" );
	bool err_loadvector = displayVector.load( shaderPath / "passthru.vert", shaderPath / "displaysvector.frag" );
	bool err_loadpixel = displayLines.load( shaderPath / "passthru.vert", shaderPath / "pixels.frag" );

	// Initialize Sequences
	setupSequences();
}

void FluidScene::update()
{
	time = ofGetElapsedTimef();

	if (debug && !step)
		return;

	updateSequence();
	updateParameters();

	vector<ccUser> u = userManager->getUserVec();

	// Do one solver step
	solver.step( u );

	if (debug) step = false;
}

void FluidScene::draw()
{
	ofBackground( 0 );

	if (p_DebugView.get()) {
		// Debug view showing all fields in a grid pattern and with a caption
		ccSolver::Grid grid = *(solver.getGrid());

		float w = width / 2;
		float h = height / 3;

		ofDrawBitmapString( "density", 0.f, 0.f + 10.f );
		solver.getDensity()->draw( 0.f, 0.f, w, h );

		// Velocity field needs special shader to be displayed, as its values range from -1.0 to 1.0 so they have to be normalized using a bias
		// For details see displayvector shader
		ofDrawBitmapString( "velocity", 0.f, h + 10.f );
		displayVector.begin();
		displayVector.setUniformTexture( "read", solver.getVelocity()->getTexture(), 1 );
		displayVector.setUniform3f( "bias", glm::vec3( 0.5, 0.5, 0.5 ) );
		displayVector.setUniform3f( "scale", glm::vec3( 0.5, 0.5, 0.5 ) );
		displayVector.setUniform2f( "gridSize", solver.getGrid()->size );
		solver.getVelocity()->draw( 0.f, h, w, h );
		displayVector.end();

		ofDrawBitmapString( "divergence", w, 0.f + 10.f );
		solver.getDivergence()->draw( w, 0.f, w, h );

		ofDrawBitmapString( "vorticity", w, h + 10.f );
		solver.getVorticity()->draw( w, h, w, h );

		ofDrawBitmapString( "pressure", 0.f, h * 2.f + 10.f );
		solver.getPressure()->draw( 0.f, h * 2.f, w, h );

	}
	else {
		// Only draw density texture
		switch (shading)
		{
		case ShadingType::DEFAULT:
			drawDefault();
			break;
		case ShadingType::PIXELS:
			drawPixelated();
			break;
		default:
			break;
		}
	}
}

// Draw using the default shader
void FluidScene::drawDefault() {
	camera.begin();
	displayScalar.begin();
	displayScalar.setUniformTexture( "read", solver.getDensity()->getTexture(), 1 );
	displayScalar.setUniform3f( "bias", glm::vec3( 0.5, 0.5, 0.5 ) );
	displayScalar.setUniform3f( "scale", glm::vec3( 0.5, 0.5, 0.5 ) );
	displayScalar.setUniform2f( "gridSize", solver.getGrid()->size );
	solver.getDensity()->draw( 0.f, 0.f, width, height );
	displayScalar.end();
	camera.end();
}

// Draw using the pixelated shader
void FluidScene::drawPixelated() {
	camera.begin();
	displayLines.begin();
	displayLines.setUniformTexture( "read", solver.getDensity()->getTexture(), 1 );
	displayLines.setUniform2f( "gridSize", solver.getGrid()->size );
	displayLines.setUniform3f( "bias", glm::vec3( 0.5, 0.5, 0.5 ) );
	displayLines.setUniform3f( "scale", glm::vec3( 0.5, 0.5, 0.5 ) );
	solver.getDensity()->draw( 0.f, 0.f, width, height );
	displayLines.end();
	camera.end();
}


///////////////
// Sequences //
///////////////
void FluidScene::setupSequences() {
	lastSequenceTime = time;
	// Standart
	sequenceMap.insert( pair<FluidScene::SequenceName, SequenceParameters>( SequenceName::Default, { 1.f,  0.002f, 0.998f, false, 0.f, false, 0.f, {1.f, 1.f, 1.f } } ) );
	// Fast
	sequenceMap.insert( pair<FluidScene::SequenceName, SequenceParameters>( SequenceName::Fast, { 0.2f,  0.001f, 0.992f, false, 0.f, false, 0.f,{1.f, 1.f, 1.f } } ) );
	// Smoke
	sequenceMap.insert( pair<FluidScene::SequenceName, SequenceParameters>( SequenceName::Smoke, { 0.5f,  0.002f, 0.998f, true, .15f, false, 0.f, {1.f, 1.f, 1.f } } ) );
	// Empty
	sequenceMap.insert( pair<FluidScene::SequenceName, SequenceParameters>( SequenceName::Empty, { 1.f,  0.002f, 0.9f, false, 0.f, false, 0.f, {0.f, 0.f, 0.f } } ) );
}

void FluidScene::updateSequence() {
	if (!p_Sequences)
		return;

	// Change sequences periodically
	if (time - lastSequenceTime > sequenceDuration) {
		//Set to a random sequence
		setSequence( randSequence() );
		cout << "Changing Sequence! Current Sequence: " << static_cast<int>(currentSequence) << endl;
	}
}

void FluidScene::setSequence( SequenceName name ) {
	lastSequene = currentSequence;
	currentSequence = name;
	lastSequenceTime = ofGetElapsedTimef();

	// Set boolean parameters at the start of the sequence
	p_ApplyVorticity.set( sequenceMap.at( currentSequence ).applyVorticity );
	p_ApplyViscosity.set( sequenceMap.at( currentSequence ).applyViscosity );
}

// Changes happening graduallyduring sequence change
void FluidScene::updateParameters() {
	if (p_Sequences.get() && time - lastSequenceTime <= sequenceTransitionDuration)
	{
		float timeSinceSequenceChange = time - lastSequenceTime;
		p_Scale.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).scale, sequenceMap.at( currentSequence ).scale ) );
		p_SplatRadius.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).splat, sequenceMap.at( currentSequence ).splat ) );
		p_Dissipation.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).dissipation, sequenceMap.at( currentSequence ).dissipation ) );
		p_Curl.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).curl, sequenceMap.at( currentSequence ).curl ) );
		p_Viscosity.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).viscosity, sequenceMap.at( currentSequence ).viscosity ) );

		// Changing color is a bit more complex
		glm::vec3 currCol = sequenceMap.at( lastSequene ).color;
		glm::vec3 targetCol = sequenceMap.at( currentSequence ).color;
		float r = ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, currCol.r, targetCol.r );
		float g = ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, currCol.g, targetCol.g );
		float b = ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, currCol.b, targetCol.b );
		p_SplatColor.set( ofFloatColor( r, g, b )
		);
	}
}

FluidScene::SequenceName FluidScene::randSequence()
{
	return static_cast<SequenceName>(rand() % NUM_SEQ);
}


//////////////////////
// Scene Transition //
//////////////////////

float FluidScene::SceneIntro()
{
	lastSequene = SequenceName::Empty;
	setSequence( randSequence() );

	//changeShading();

	return sequenceTransitionDuration;
}

float FluidScene::SceneOutro()
{
	setSequence( SequenceName::Empty );

	return sequenceTransitionDuration;
}

// Change shading type, right now just switches between outlined and metaball shading
void FluidScene::changeShading() {
	shading = shading ? ShadingType::DEFAULT : ShadingType::PIXELS;
}


//////////////////
// Input Events //
//////////////////

void FluidScene::keyPressed( int key ) {

	//std::cout << key << std::endl;
	switch (key) {
	case ofKey::OF_KEY_SHIFT:
		camera.enableMouseInput();
		break;
	case 'r':
		resetCamera();

	case 'd':
		debug = !debug;
		cout << "Debug: " << (debug ? "On" : "Off") << endl;
		break;
	case 32:
		if (debug) step = true;
		break;
	case's':
		changeShading();
		break;
	}
}

void FluidScene::keyReleased( int key ) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();
	}
}