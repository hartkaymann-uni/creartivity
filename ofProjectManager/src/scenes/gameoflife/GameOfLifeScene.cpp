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
	bool load1 = updateCells.load( shader_path / "passthru.vert", shader_path / "gol.frag" );
	bool load2 = instancedShader.load( shader_path / "renderInstanced.vert", shader_path / "renderInstanced.frag" );
	bool load3 = outlineShader.load( shader_path / "renderInstanced.vert", shader_path / "outline.frag" );
}


void GameOfLifeScene::setup()
{
	allocateCellBuffer( n_cells_x, n_cells_y );

	cellOffset = calculateSphereRadius( ofVec2f( n_cells_x, n_cells_y ) );

	// Set all Parameters once
	dimensions.set( "dimensions", ofVec2f( n_cells_x, n_cells_y ), ofVec2f( 1.0, 1.0 ), ofVec2f( n_cells_x * 10.0, n_cells_y * 10.0 ) );
	evolutionFactor.set( "evolutionFac", 0.05, 0.0, 0.25 );
	sphereResolution.set( "circleRes", 20, 1, 100 );
	sphereRadius.set( "radius", cellOffset, 0.0, cellOffset * 5.0 );
	dataSrcSize.set( "srcSize", 0, 0, 9 );
	mouseRadius.set( "mouseRad", 5, 0, 10 );
	mouseStrength.set( "mouseStr", 0.1, 0.0, 1.0 );
	jiggleFactor.set( "jiggle", 1.0, 0.0, 10.0 );
	fluctuateParameters.set( "fluctuate", false );

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
	gui.add( fluctuateParameters );
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


void GameOfLifeScene::update()
{
	time = ofGetElapsedTimef();

	updateUserPositions();

	// Gradually change parameters
	if (fluctuateParameters.get()) {
		randomizeFloatParameter( evolutionFactor );
		randomizeFloatParameter( sphereRadius );
		randomizeFloatParameter( jiggleFactor );
	}

	cellPingPong.dst->begin();
	ofClear( 0 );
	updateCells.begin();
	updateCells.setUniforms( shaderUniforms );
	updateCells.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	updateCells.setUniform2f( "resolution", (float)n_cells_x, (float)n_cells_y );
	updateCells.setUniform2f( "screen", (float)width, (float)height );
	updateCells.setUniform1f( "offset", cellOffset );
	updateCells.setUniform1i( "mouseDown", mouseIsDown );
	updateCells.setUniform3f( "mousePos", mousePosition );
	updateCells.setUniform2fv( "hands", &user_positions[0].x, sizeof( ofVec2f ) * 10 );

	// Draw cell texture to call shaders, logic happens in shaders
	cellPingPong.src->draw( 0, 0 );

	updateCells.end();

	cellPingPong.dst->end();
	cellPingPong.swap();

	//std::cout << camera.getPosition() << std::endl;
}

void GameOfLifeScene::draw()
{
	ofBackground( 0 );

	ofPushStyle();

	//ofSetColor( 100, 255, 255 );

	camera.begin();
	//glEnable( GL_CULL_FACE );
	//glCullFace( GL_BACK );

	// Draw outlines with stencil testing
	glEnable( GL_STENCIL_TEST );
	glDepthFunc( GL_LESS );
	glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );

	glClearStencil( 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	instancedShader.begin();
	instancedShader.setUniforms( shaderUniforms );
	instancedShader.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	instancedShader.setUniform2f( "resolution", (float)n_cells_x, (float)n_cells_y );
	instancedShader.setUniform2f( "screen", (float)width, (float)height );
	instancedShader.setUniform1f( "offset", cellOffset );
	instancedShader.setUniform1f( "time", time );

	glStencilFunc( GL_ALWAYS, 1, 0xFF );
	glStencilMask( 0xFF );
	vboSphere.drawInstanced( OF_MESH_FILL, n_cells_x * n_cells_y );

	instancedShader.end();

	outlineShader.begin();
	outlineShader.setUniforms( shaderUniforms );
	outlineShader.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	outlineShader.setUniform2f( "resolution", (float)n_cells_x, (float)n_cells_y );
	outlineShader.setUniform2f( "screen", (float)width, (float)height );
	instancedShader.setUniform1f( "offset", cellOffset );
	outlineShader.setUniform1f( "time", time );

	// Scale up spheres for second draw
	float scaledRadius = sphereRadius.get() * 1.1f;
	outlineShader.setUniform1f( "radius", scaledRadius );

	glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
	glStencilMask( 0x00 );
	glDisable( GL_DEPTH_TEST );
	vboSphere.drawInstanced( OF_MESH_FILL, n_cells_x * n_cells_y );

	glStencilMask( 0xFF );
	glStencilFunc( GL_ALWAYS, 0, 0xFF );

	outlineShader.end();

	//glDisable( GL_CULL_FACE );
	ofEnableAlphaBlending();

	ofPopStyle();

	// Draw secondary objects
#if 0
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
#endif
	ofNoFill();
	ofSetColor( 255 );
	ofDrawBox( ofVec3f( width / 2, height / 2, 0.0 ), width, height, sphereRadius );

	camera.end();

	// Draw overlay
	cellPingPong.dst->draw( 0, 0, width / (10 - dataSrcSize), height / (10 - dataSrcSize) );

	//ofDrawBitmapString( receiver->getConnectionStatus(), 10, ofGetHeight() - 20 );

}

void GameOfLifeScene::reset()
{
	time = 0.f;
	ofResetElapsedTimeCounter();
}

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
