#include "GameOfLifeScene.h"

#include <random>

GameOfLifeScene::GameOfLifeScene( int cells_x, int cells_y )
	: ccScene( "GameOfLife" ),
	n_cells_x( cells_x ),
	n_cells_y( cells_y ),
	mouseIsDown( false ),
	mousePosition( 0.f, 0.f, 0.f ),
	time( 0.f )
{
	// Load Shaders
	filesystem::path shader_path = getShaderPath();
	bool load1 = logicShader.load( shader_path / "passthru.vert", shader_path / "gol.frag" );
	bool load2 = instancedShader.load( shader_path / "renderInstanced.vert", shader_path / "renderInstanced.frag" );
	bool load3 = outlineShader.load( shader_path / "renderInstanced.vert", shader_path / "outline.frag" );
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
	dimensions.set( "dimensions", ofVec2f( n_cells_x, n_cells_y ), ofVec2f( 1.0, 1.0 ), ofVec2f( n_cells_x * 10.0, n_cells_y * 10.0 ) );
	evolutionFactor.set( "evolutionFac", 0.05, 0.0, 0.25 );
	sphereResolution.set( "circleRes", 20, 1, 100 );
	sphereRadius.set( "radius", cellOffset, 0.0, cellOffset * 5.0 );
	dataSrcSize.set( "srcSize", 0, 0, 9 );
	mouseRadius.set( "mouseRad", 5, 0, 10 );
	mouseStrength.set( "mouseStr", 0.1, 0.0, 1.0 );
	jiggleFactor.set( "jiggle", 1.0, 0.0, 10.0 );
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
	gui.setPosition( width - gui.getWidth() - 10, height - gui.getHeight() - 10 );

	ofSpherePrimitive sphere;
	sphere.set( 1.0, sphereResolution );
	vboSphere = sphere.getMesh();
}

void GameOfLifeScene::allocateCellBuffer( int rows, int cols ) {
	ofSeedRandom();
	vector<float> cells( rows * cols * 3 );
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
	lastSequene = Sequence::Default;
	currentSequence = Sequence::Default;
	lastSequenceTime = time;
	// Standart
	sequenceMap.insert( pair<GameOfLifeScene::Sequence, SequenceParameters>( Sequence::Default, { 0.05, cellOffset, 1.0 } ) );
	// No Jiggle
	sequenceMap.insert( pair<GameOfLifeScene::Sequence, SequenceParameters>( Sequence::NoJiggle, { 0.05, cellOffset, 0.0 } ) );
	// Big cells
	sequenceMap.insert( pair<GameOfLifeScene::Sequence, SequenceParameters>( Sequence::BigCells, { 0.03, cellOffset * 4.f, 3.0 } ) );
	// Small cell
	sequenceMap.insert( pair<GameOfLifeScene::Sequence, SequenceParameters>( Sequence::SmallCells, { 0.085, cellOffset * .5f, 0.5 } ) );
	// Fast evolution
	sequenceMap.insert( pair<GameOfLifeScene::Sequence, SequenceParameters>( Sequence::FastEvolution, { 0.15, cellOffset, 2.0 } ) );
	// Slow evolution
	sequenceMap.insert( pair<GameOfLifeScene::Sequence, SequenceParameters>( Sequence::SlowEvolution, { 0.015, cellOffset * 0.75f, 1.5 } ) );
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

		lastSequene = currentSequence;
		currentSequence = static_cast<Sequence>(rand() % NUM_SEQ);

		cout << "Chaning Sequence! Current Sequence: " << static_cast<int>(currentSequence) << endl;

		lastSequenceTime = time;
	}
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

	ofNoFill();
	ofSetColor( 255 );
	ofDrawBox( ofVec3f( width / 2, height / 2, 0.0 ), width, height, sphereRadius );

	camera.end();

	// Draw overlay
	if (dataSrcSize > 0)
	{
		cellPingPong.dst->draw( 0, 0, width / (10 - dataSrcSize), height / (10 - dataSrcSize) );
	}
	//ofDrawBitmapString( receiver->getConnectionStatus(), 10, ofGetHeight() - 20 );
}

// Draw outlines with stencil testing
void GameOfLifeScene::drawOutlined( const ofVboMesh& mesh, ofShader& instance, ofShader& outline) {
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
void GameOfLifeScene::keyPressed( int key ) {

	// std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
	}
	else if (key == 'r' || key == 'R') {
		resetCamera();
	}
}

void GameOfLifeScene::keyReleased( int key ) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();
	}
}

void GameOfLifeScene::mousePressed( int x, int y, int button )
{
	mouseIsDown = true;
	mousePosition.set( getProjectedPosition( ofVec3f( x, y, 0.0 ) ) );
}

void GameOfLifeScene::mouseReleased( int x, int y, int button )
{
	mouseIsDown = false;
}

void GameOfLifeScene::mouseDragged( int x, int y, int button )
{
	if (mouseIsDown)
		mousePosition.set( getProjectedPosition( ofVec3f( x, y, 0.0 ) ) );
}

void GameOfLifeScene::windowResized( int w, int h ) {
	width = ofGetWidth();
	height = ofGetHeight();

	ofVec2f dim = dimensions.get();
	handleDimensionsChanged( dim );
}

