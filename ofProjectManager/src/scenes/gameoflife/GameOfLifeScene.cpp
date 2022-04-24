#include "GameOfLifeScene.h"

#include <random>

namespace gol {
	GameOfLifeScene::GameOfLifeScene( int cells_x, int cells_y )
		: ccScene( "GameOfLife" ),
		n_cells_x( cells_x ),
		n_cells_y( cells_y ),
		time( 0.f ),
		cellOffset( 0.f ),
		sequenceDuration( 10.f ),
		sequenceTransitionDuration( 1.f ),
		lastSequene( SequenceName::Empty ),
		currentSequence( SequenceName::Empty ),
		lastSequenceTime( 0.f )
	{
		// Load Shaders
		filesystem::path shader_path = getShaderPath();
		string module = "GameOfLife Setup";

		bool err_logic = logicShader.load( shader_path / "passthru.vert", shader_path / "gol.frag" );
		if (!err_logic) {
			(void)ofLogError( module, "Failed to load logic shader!" );
		}

		bool err_instanced = instancedShader.load( shader_path / "renderInstanced.vert", shader_path / "renderInstanced.frag" );
		if (!err_instanced) {
			(void)ofLogError( module, "Failed to load logic shader!" );
		}

		bool err_outline = outlineShader.load( shader_path / "renderInstanced.vert", shader_path / "outline.frag" );
		if (!err_outline) {
			(void)ofLogError( module, "Failed to load outline shader!" );
		}
	}

	///////////
	// Setup //
	///////////
	void GameOfLifeScene::setup()
	{
		allocateCellBuffer( n_cells_x, n_cells_y );

		cellOffset = calculateSphereRadius( ofVec2f( n_cells_x, n_cells_y ) );

		initSequences();

		// Set all Parameters once
		dimensions.set( "dimensions", ofVec2f( n_cells_x, n_cells_y ), ofVec2f( 1.f, 1.f ), ofVec2f( n_cells_x * 10.f, n_cells_y * 10.f ) );
		evolutionFactor.set( "evolutionFac", 0.05f, 0.f, 0.25f );
		sphereResolution.set( "circleRes", 20, 1, 100 );
		sphereRadius.set( "radius", cellOffset, 0.f, cellOffset * 5.f );
		dataSrcSize.set( "srcSize", 0.f, 0.f, 9.f );
		mouseRadius.set( "mouseRad", 5.f, 0.f, 10.f );
		mouseStrength.set( "mouseStr", 0.5f, 0.f, 1.f );
		jiggleFactor.set( "jiggle", 1.f, 0.f, 10.f );
		runSequences.set( "Run sequences", true );

		sphereResolution.addListener( this, &GameOfLifeScene::handleSphereResolutionChanged );
		dimensions.addListener( this, &GameOfLifeScene::handleDimensionsChanged );

		shaderUniforms.setName( "Shader Parameters" );
		shaderUniforms.add( evolutionFactor );
		shaderUniforms.add( sphereRadius );
		shaderUniforms.add( jiggleFactor );
		shaderUniforms.add( mouseRadius );
		shaderUniforms.add( mouseStrength );
		shaderUniforms.add( dataSrcSize );

		gui.add( shaderUniforms );
		gui.add( sphereResolution );
		gui.add( runSequences );
		gui.add( dimensions );
		gui.setPosition( width - gui.getWidth() - 10.f, height - gui.getHeight() - 10.f );

		ofSpherePrimitive sphere;
		sphere.set( 1.0, sphereResolution );
		vboSphere = sphere.getMesh();
	}

	void GameOfLifeScene::allocateCellBuffer( int rows, int cols ) {
		ofSeedRandom();
		int n_cells = rows * cols * 3;
		vector<float> cells( n_cells );
		for (size_t x = 0; x < rows; x++) {
			for (size_t y = 0; y < cols; y++) {
				size_t i = x * cols + y;
				float initialValue = ofRandom( 1.0 );

				cells[i * 3 + 0] = initialValue;
				cells[i * 3 + 1] = 0.0;
				cells[i * 3 + 2] = 0.0;
			}
		}

		// Load data into the FBO's texture
		cellPingPong.allocate( rows, n_cells_y, GL_RGB32F );
		cellPingPong.src->getTexture().loadData( cells.data(), rows, cols, GL_RGB );
		cellPingPong.dst->getTexture().loadData( cells.data(), rows, cols, GL_RGB );
	}


	void GameOfLifeScene::initSequences()
	{
		lastSequenceTime = time;
		// Standart
		sequenceMap.insert( pair<GameOfLifeScene::SequenceName, SequenceParameters>( SequenceName::Default, { 0.05, cellOffset, 1.0 } ) );
		// No Jiggle
		sequenceMap.insert( pair<GameOfLifeScene::SequenceName, SequenceParameters>( SequenceName::NoJiggle, { 0.05, cellOffset, 0.0 } ) );
		// Big cells
		sequenceMap.insert( pair<GameOfLifeScene::SequenceName, SequenceParameters>( SequenceName::BigCells, { 0.03, cellOffset * 4.f, 3.0 } ) );
		// Small cell
		sequenceMap.insert( pair<GameOfLifeScene::SequenceName, SequenceParameters>( SequenceName::SmallCells, { 0.085, cellOffset * .75f, 0.5 } ) );
		// Fast evolution
		sequenceMap.insert( pair<GameOfLifeScene::SequenceName, SequenceParameters>( SequenceName::FastEvolution, { 0.15, cellOffset, 2.0 } ) );
		// Slow evolution
		sequenceMap.insert( pair<GameOfLifeScene::SequenceName, SequenceParameters>( SequenceName::SlowEvolution, { 0.015, cellOffset, 1.5 } ) );
		// Empty Sequence for Intro and Outro
		sequenceMap.insert( pair<GameOfLifeScene::SequenceName, SequenceParameters>( SequenceName::Empty, { 0.0, 0.0, 0.0 } ) );
	}

	////////////
	// Update //
	////////////
	void GameOfLifeScene::update()
	{
		time = ofGetElapsedTimef();

		updateUserPositions();
		updateSequence();
		updateParameters();

		// Main logic
		cellPingPong.dst->begin();
		ofClear( 0 );
		logicShader.begin();
		logicShader.setUniforms( shaderUniforms );
		logicShader.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
		logicShader.setUniform2f( "resolution", (float)n_cells_x, (float)n_cells_y );
		logicShader.setUniform2f( "screen", (float)width, (float)height );
		logicShader.setUniform1f( "offset", cellOffset );
		logicShader.setUniform1i( "mouseDown", mouseIsDown );
		logicShader.setUniform3f( "mousePos", mousePosition );
		logicShader.setUniform2fv( "hands", &user_positions[0].x, sizeof( ofVec2f ) * 10 );

		// Draw cell texture to call shaders, logic happens in shaders
		cellPingPong.src->draw( 0, 0 );

		logicShader.end();

		// Ping pong
		cellPingPong.dst->end();
		cellPingPong.swap();
	}

	// Handles sequence changes
	void GameOfLifeScene::updateSequence() {
		if (!runSequences)
			return;

		// Change sequences periodically
		if (time - lastSequenceTime > sequenceDuration) {
			//Set to a random sequence
			setSequence( randSequence() );
			cout << "Changing Sequence! Current Sequence: " << static_cast<int>(currentSequence) << endl;
		}
	}

	void GameOfLifeScene::setSequence( SequenceName name )
	{
		lastSequene = currentSequence;
		currentSequence = name;
		lastSequenceTime = ofGetElapsedTimef();
	}

	GameOfLifeScene::SequenceName GameOfLifeScene::randSequence()
	{
		return static_cast<SequenceName>(rand() % NUM_SEQ);
	}

	// Updates parameters e.g. after sequence change
	void GameOfLifeScene::updateParameters() {
		if (runSequences.get() && time - lastSequenceTime <= sequenceTransitionDuration)
		{
			float timeSinceSequenceChange = time - lastSequenceTime;
			evolutionFactor.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).evolution, sequenceMap.at( currentSequence ).evolution ) );
			sphereRadius.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).radius, sequenceMap.at( currentSequence ).radius ) );
			jiggleFactor.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).jiggle, sequenceMap.at( currentSequence ).jiggle ) );
		}
	}

	///////////////
	// Rendering //
	///////////////
	void GameOfLifeScene::draw()
	{
		ofBackground( 0 );

		camera.begin();

		drawOutlined( vboSphere, instancedShader, outlineShader );

		// Draw secondary objects
#if 0
		ofPushStyle();
		ofFill();
		ofSetColor( ofColor::red );
		std::unique_ptr<std::map<int, user>> users = receiver->getUsers();
		std::map<int, user>::iterator it = users->begin();
		std::map<int, user>::iterator itEnd = users->end();
		while (it != itEnd) {
			float xl = it->second.positionLeft.x * width;
			float yl = it->second.positionLeft.y * height;
			float xr = it->second.positionRight.x * width;
			float yr = it->second.positionRight.y * height;

			ofDrawCircle( getProjectedPosition( ofVec3f( xl, yl, 0.f ) ), 10 );
			ofDrawCircle( getProjectedPosition( ofVec3f( xr, yr, 0.f ) ), 10 );

			it++;
		}
		ofPopStyle();
#endif

		// Draw overlay
		if (dataSrcSize > 0)
		{
			cellPingPong.dst->draw( 0, 0, width / (10 - dataSrcSize), height / (10 - dataSrcSize) );
		}
		camera.end();
	}

	// Draw outlines with stencil testing
	void GameOfLifeScene::drawOutlined( ofVboMesh& mesh, ofShader& instance, ofShader& outline ) {
		ofPushStyle();

		glEnable( GL_STENCIL_TEST );
		glDepthFunc( GL_LESS );
		glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );

		glClearStencil( 0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

		instance.begin();
		instance.setUniforms( shaderUniforms );
		instance.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
		instance.setUniform2f( "resolution", (float)n_cells_x, (float)n_cells_y );
		instance.setUniform2f( "screen", (float)width, (float)height );
		instance.setUniform1f( "offset", cellOffset );
		instance.setUniform1f( "time", time );

		glStencilFunc( GL_ALWAYS, 1, 0xFF );
		glStencilMask( 0xFF );
		mesh.drawInstanced( OF_MESH_FILL, n_cells_x * n_cells_y );

		instance.end();

		outline.begin();
		outline.setUniforms( shaderUniforms );
		outline.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
		outline.setUniform2f( "resolution", (float)n_cells_x, (float)n_cells_y );
		outline.setUniform2f( "screen", (float)width, (float)height );
		outline.setUniform1f( "offset", cellOffset );
		outline.setUniform1f( "time", time );

		// Scale up spheres for second draw
		float scaledRadius = sphereRadius.get() * 1.1f;
		outline.setUniform1f( "radius", scaledRadius );

		glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
		glStencilMask( 0x00 );
		glDisable( GL_DEPTH_TEST );
		mesh.drawInstanced( OF_MESH_FILL, n_cells_x * n_cells_y );

		glStencilMask( 0xFF );
		glStencilFunc( GL_ALWAYS, 0, 0xFF ); 

		outline.end();

		ofEnableAlphaBlending();

		ofPopStyle();

	}

	void GameOfLifeScene::reset()
	{
		time = 0.f;
		ofResetElapsedTimeCounter();
	}


	//////////////////////
	// Scene Transition //
	//////////////////////

	float GameOfLifeScene::SceneIntro() {
		lastSequene = SequenceName::Empty;
		setSequence( randSequence() );

		allocateCellBuffer( n_cells_x, n_cells_y );

		return sequenceTransitionDuration;
	}

	float GameOfLifeScene::SceneOutro() {
		setSequence( SequenceName::Empty );

		return sequenceTransitionDuration;
	}


	///////////////////////
	// Function Handlers //
	///////////////////////
	void GameOfLifeScene::handleSphereResolutionChanged( int& val )
	{
		ofSpherePrimitive sphere;
		sphere.set( 1.0, sphereResolution );
		vboSphere = sphere.getMesh();
	}

	void GameOfLifeScene::handleDimensionsChanged( ofVec2f& value )
	{
		sphereRadius = (float)width / (float)value.x;

		n_cells_x = width / sphereRadius;
		n_cells_y = height / sphereRadius;

		cellOffset = sphereRadius.get();
		dimensions.setWithoutEventNotifications( ofVec2f( n_cells_x, n_cells_y ) );

		allocateCellBuffer( n_cells_x, n_cells_y );
		reset();
	}

	float GameOfLifeScene::calculateSphereRadius( ofVec2f dim ) {
		return std::min( (float)width / (float)dim.x, (float)height / (float)dim.y );
	}

	//////////////////
	// Input Events //
	//////////////////

	void GameOfLifeScene::windowResized( int w, int h ) {
		width = ofGetWidth();
		height = ofGetHeight();

		ofVec2f dim = dimensions.get();
		handleDimensionsChanged( dim );
	}
}