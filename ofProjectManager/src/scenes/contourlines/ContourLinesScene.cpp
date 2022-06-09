#include "ContourLinesScene.h"

namespace contour {
	ContourLinesScene::ContourLinesScene( int w, int h ) : ccScene( "ContourLines" ),
		time( 0.f ),
		sceneTime(0.f),
		grid( { w, h } ),
		sequenceDuration( 5.f ),
		sequenceTransitionDuration( 3.f ),
		lastSequene( SequenceName::Empty ),
		currentSequence( SequenceName::Default ),
		lastSequenceTime( 0.f )
	{}

	void ContourLinesScene::setup()
	{
		// Load Shader
		filesystem::path shader_path = getShaderPath();
		bool err_cont = contourLineShader.load( shader_path / "contour.vert", shader_path / "contour.frag" );
		bool err_splat = splatShader.load( shader_path / "passthru.vert", shader_path / "splat.frag" );
		bool err_sub = subtractShader.load( shader_path / "passthru.vert", shader_path / "subtract.frag" );

		// Create mesh
		plane = ofPlanePrimitive( width, height, grid.x, grid.y );
		plane.setPosition( width / 2, height / 2, 0.f );

		ofDisableArbTex();
		interaction.allocate( grid, GL_R32F );
		ofEnableArbTex();

		// Setup gui and parameters
		terrainUniforms.setName( "Terrain" );
		terrainUniforms.add( p_Speed.set( "u_speed", 0.01, 0.00, 0.5 ) );
		terrainUniforms.add( p_Scale.set( "u_scale", 0.005, 0.0, 0.05 ) );
		terrainUniforms.add( p_Amplitude.set( "u_amplitude", 5.0, 0.0, 20.0 ) );
		terrainUniforms.add( p_Thickness.set( "u_thickness", 0.2, 0.0, 1.0 ) );
		terrainUniforms.add( p_Lacunarity.set( "u_lacunarity", 1.0, 0.0, 5.0 ) );
		terrainUniforms.add( p_Persistance.set( "u_persistance", 0.25, 0.0, 1.0 ) );
		terrainUniforms.add( p_Sequences.set( "Run Sequences", true ) );

		mouseUniforms.setName( "Mouse" );
		mouseUniforms.add( p_MouseRadius.set( "u_radius", 0.003, 0.0, 0.01 ) );
		mouseUniforms.add( p_MouseStrength.set( "u_strength", 0.2, 0.0, 1.0 ) );

//		lightUniforms.add( p_MoveLight.set( "u_moving", false ) );
//		lightUniforms.setName( "Light" );

		gui.add( terrainUniforms );
		gui.add( mouseUniforms );
//		gui.add( lightUniforms );

		gui.setPosition( width - gui.getWidth() - 10, height - gui.getHeight() - 10 );

		initSequences();
	}

	void ContourLinesScene::update()
	{
		time = ofGetElapsedTimef();
		float dt = ofGetLastFrameTime();
		sceneTime += dt * p_Speed.get();
		//cout << sceneTime << endl;

		updateSequence();
		updateParameters();

		// Flatten terrain
		subtractShader.begin();
		subtractShader.setUniformTexture( "read", interaction.read->getTexture(), 2 );
		subtractShader.setUniform2f( "grid", grid );

		interaction.write->begin();
		plane.draw();
		interaction.write->end();
		interaction.swap();

		subtractShader.end();

		// Apply interaction for all users
		vector<ccUser> u = userManager->getUserVec();
		for ( vector<ccUser>::iterator it = u.begin(); it != u.end(); it++ ) {
			ccUser user = *it;
			// Only apply interaction if user is moving
			if ( glm::length( user.getMotions().first ) > 0.f )
				splat( user.getPositons().first );
			if ( glm::length( user.getMotions().second ) > 0.f )
				splat( user.getPositons().second );
		}
	}

	void ContourLinesScene::draw()
	{
		camera.begin();
		{
			contourLineShader.begin();
			{
				contourLineShader.setUniformTexture( "interaction", interaction.read->getTexture(), 2 );
				contourLineShader.setUniform2f( "u_resolution", { width, height } );
				contourLineShader.setUniform1f( "u_time", sceneTime );
				contourLineShader.setUniform2f( "u_mouse", ofGetMouseX(), height - ofGetMouseY() );
				contourLineShader.setUniforms( terrainUniforms );
//				contourLineShader.setUniforms( lightUniforms );

				ofDisableAlphaBlending();

				if ( !wireframeShading ) plane.draw();
				else plane.drawWireframe();
			}
			contourLineShader.end();
		}
		camera.end();

	}

	void ContourLinesScene::splat( glm::vec3 point ) {
		splatShader.begin();

		splatShader.setUniformTexture( "read", interaction.read->getTexture(), 2 );
		splatShader.setUniform2f( "grid", grid );
		splatShader.setUniform2f( "u_point", point );
		splatShader.setUniforms( mouseUniforms );

		interaction.write->begin();
		ofClear( 0 );
		ofFill();
		plane.draw();
		interaction.write->end();
		interaction.swap();

		splatShader.end();
	}

	void ContourLinesScene::keyPressed( int key )
	{
		// reset the camera to the middle of the scene
		switch ( key ) {
		case 'r':
			camera.reset();
			camera.setPosition( width / 2, height / 2, (width + height) / 2 );
			break;
		case ofKey::OF_KEY_SHIFT:
			camera.enableMouseInput();
			break;
		case 's':
			changeShading();
			break;
		}
	}

	void ContourLinesScene::keyReleased( int key )
	{
		if ( key == ofKey::OF_KEY_SHIFT ) {
			camera.disableMouseInput();
		}
	}

	void ContourLinesScene::changeShading() {
		wireframeShading = !wireframeShading;
	}

	void ContourLinesScene::windowResized( int w, int h ) {
		width = w;
		height = h;

		plane = ofPlanePrimitive( width, height, grid.x, grid.y );
		plane.setPosition( width / 2, height / 2, 0.f );

		resetCamera();
	}


	///////////////
	// Sequences //
	///////////////	

	/*	Define values for different sequences and create sequence map */
	/*
	*	type	name
	*	float	speed
	*	float	scale
	*	float	amplitude
	*	float	thickness
	*	float	lacunarity
	*	float	persistance
	*/
	void ContourLinesScene::initSequences()
	{
		lastSequenceTime = time;
		// Standart
		sequenceMap.insert( pair<ContourLinesScene::SequenceName, SequenceParameters>( SequenceName::Default, { 0.01, 0.005, 6.0,  0.2, 1.0, 0.25 } ) );
		// Fast change
		sequenceMap.insert( pair<ContourLinesScene::SequenceName, SequenceParameters>( SequenceName::Fast, { 0.1, 0.005, 6.0,  0.2, 1.0, 0.25 } ) );
		// Big scale
		sequenceMap.insert( pair<ContourLinesScene::SequenceName, SequenceParameters>( SequenceName::Big, { 0.01, 0.02, 4.0,  0.2, 1.0, 0.25 } ) );
		// High Amplitude
		sequenceMap.insert( pair<ContourLinesScene::SequenceName, SequenceParameters>( SequenceName::HighAmplitude, { 0.01, 0.005, 10.0,  0.2, 1.0, 0.25 } ) );
		// Rough terrain
		sequenceMap.insert( pair<ContourLinesScene::SequenceName, SequenceParameters>( SequenceName::Rough, { 0.01, 0.005, 6.0, 0.2, 2.5, 0.25 } ) );
		// Empty Sequence for Intro and Outro
		sequenceMap.insert( pair<ContourLinesScene::SequenceName, SequenceParameters>( SequenceName::Empty, { 0.01, 0.000, 0.1,  0.0, 0.0, 0.0 } ) );
	}

	// Handles sequence changes
	void ContourLinesScene::updateSequence() {
		if ( !p_Sequences )
			return;

		// Change sequences periodically
		if ( time - lastSequenceTime > sequenceDuration ) {
			//Set to a random sequence
			setSequence( randSequence() );
			cout << "Changing Sequence! Current Sequence: " << static_cast<int>(currentSequence) << endl;
		}
	}

	void ContourLinesScene::setSequence( SequenceName name )
	{
		lastSequene = currentSequence;
		currentSequence = name;
		lastSequenceTime = ofGetElapsedTimef();
	}

	// Updates parameters e.g. after sequence change
	void ContourLinesScene::updateParameters() {
		if ( p_Sequences.get() && time - lastSequenceTime <= sequenceTransitionDuration )
		{
			float timeSinceSequenceChange = time - lastSequenceTime;
			if ( timeSinceSequenceChange < 0 ) {
				return;
			}
			p_Speed.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).speed, sequenceMap.at( currentSequence ).speed ) );
			p_Scale.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).scale, sequenceMap.at( currentSequence ).scale ) );
			p_Amplitude.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).amplitude, sequenceMap.at( currentSequence ).amplitude ) );
			p_Thickness.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).thickness, sequenceMap.at( currentSequence ).thickness ) );
			p_Lacunarity.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).lacunarity, sequenceMap.at( currentSequence ).lacunarity ) );
			p_Persistance.set( ofMap( timeSinceSequenceChange, 0.0, sequenceTransitionDuration, sequenceMap.at( lastSequene ).persistance, sequenceMap.at( currentSequence ).persistance ) );
		}
	}

	ContourLinesScene::SequenceName ContourLinesScene::randSequence()
	{
		return static_cast<SequenceName>(rand() % NUM_SEQ);
	}

	//////////////////////
	// Scene Transition //
	//////////////////////

	float ContourLinesScene::SceneIntro() {
		lastSequene = SequenceName::Empty;
		setSequence( randSequence() );

		return sequenceTransitionDuration;
	}

	float ContourLinesScene::SceneOutro() {
		setSequence( SequenceName::Empty );
		return sequenceTransitionDuration;
	}
}