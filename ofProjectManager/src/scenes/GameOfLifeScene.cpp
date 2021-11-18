#include "GameOfLifeScene.h"

#include <random>

GameOfLifeScene::GameOfLifeScene() : GameOfLifeScene( 102, 77 ) {}

GameOfLifeScene::GameOfLifeScene( int cells_x, int cells_y )
	: ofxFadeScene( "GameOfLife" ),
	width( ofGetWindowWidth() ),
	height( ofGetWindowHeight() ),
	N_CELLS_X( cells_x ),
	N_CELLS_Y( cells_y ),
	mouseIsDown( false ),
	mousePosition( 0.f, 0.f, 0.f)
{
	setSingleSetup( false );
	setFade( 1000, 1000 );
}

void GameOfLifeScene::setup()
{
	// Load Shaders
	filesystem::path shader_path( "../../res/shader" );
	bool loadUpdateShader = updateCells.load( shader_path / "passthru.vert", shader_path / "gol.frag" );
	bool loadRenderShader = updateRender.load( shader_path / "render.vert", shader_path / "render.frag" /*, shader_path / "render.geom" */ );
	bool loadInstancedShader = instancedShader.load( shader_path / "renderInstanced.vert", shader_path / "renderInstanced.frag" );

	camera.disableMouseInput();
	camera.setupPerspective();
	camera.setPosition( 0, 0, 500 );
	camera.setFarClip( ofGetWidth() * 10 );

	lightPos = ofVec3f( 50, 50, 50 );
	light.setPointLight();
	light.setup();
	light.enable();
	light.setPosition( lightPos );

	materialColor = ofColor( 50.f, 255.f, 128.f );

	// Make array of float pixels with cell data
	// Currently only R value is be used, cells can only either be true (R > .5) or false (R =< .5)
	ofSeedRandom();
	vector<float> cells( N_CELLS_X * N_CELLS_Y * 3 );
	for (size_t x = 0; x < N_CELLS_X; x++) {
		for (size_t y = 0; y < N_CELLS_Y; y++) {
			size_t i = x * N_CELLS_Y + y;
			float initialValue = ofRandom( 1.0 );
			// initialValue = (y % 2 == 0) ? 1.0 : 0.0;

			cells[i * 3 + 0] = initialValue;
			cells[i * 3 + 1] = 0.0;
			cells[i * 3 + 2] = 0.0;
		}
	}

	// Load data into the FBO's texture
	cellPingPong.allocate( N_CELLS_X, N_CELLS_Y, GL_RGB32F );
	cellPingPong.src->getTexture().loadData( cells.data(), N_CELLS_X, N_CELLS_Y, GL_RGB );
	cellPingPong.dst->getTexture().loadData( cells.data(), N_CELLS_X, N_CELLS_Y, GL_RGB );

	// Allocate the render FBO
	renderFBO.allocate( width, height, GL_RGB );
	renderFBO.begin();
	ofClear( 255 );
	renderFBO.end();

	// Set all Parameters once
	evolutionFactor.set( "evolutionFac", 0.0, 0.0, 1.0 );
	sphereResolution.set( "circleRes", 10, 1, 100 );
	sphereRadius.set( "radius", 4.0, 0.0, 10.0 );
	cellSize.set( "size", 10.0, 1.0, 10.0 );
	dataSrcSize.set( "srcSize", 0, 0, 9 );
	mouseRadius.set( "mouseRad", 2.5, 0, 10 );

	sphereResolution.addListener( this, &GameOfLifeScene::handleSphereResolutionChanged );
	cellSize.addListener( this, &GameOfLifeScene::handleSphereRadiusChanged );

	gui.setup();
	shaderUniforms.setName( "Shader Parameters" );
	shaderUniforms.add( evolutionFactor );
	shaderUniforms.add( sphereResolution );
	shaderUniforms.add( cellSize );
	shaderUniforms.add( sphereRadius );
	shaderUniforms.add( dataSrcSize );
	shaderUniforms.add( mouseRadius );
	gui.add( shaderUniforms );
	gui.setPosition( width - gui.getWidth() - 10, height - gui.getHeight() - 10 );

	vboGrid.setMode( OF_PRIMITIVE_TRIANGLES );
	for (int y = 0; y < N_CELLS_Y; y++) {
		for (int x = 0; x < N_CELLS_X; x++) {
			vboGrid.addVertex( { x * cellSize, y * cellSize, 0 } );
			vboGrid.addTexCoord( { x, y } );
		}
	}
	for (int y = 0; y < N_CELLS_Y - 1; y++) {
		for (int x = 0; x < N_CELLS_X - 1; x++) {
			vboGrid.addIndex( x + y * N_CELLS_X );
			vboGrid.addIndex( (x + 1) + y * N_CELLS_X );
			vboGrid.addIndex( x + (y + 1) * N_CELLS_X );

			vboGrid.addIndex( (x + 1) + y * N_CELLS_X );
			vboGrid.addIndex( (x + 1) + (y + 1) * N_CELLS_X );
			vboGrid.addIndex( x + (y + 1) * N_CELLS_X );
		}
	}

	axisMesh = ofMesh::axis();

	ofSpherePrimitive sphere;
	sphere.set( cellSize, 10 );
	vboSphere = sphere.getMesh();
}

void GameOfLifeScene::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	cellPingPong.dst->begin();
	ofClear( 0 );
	updateCells.begin();
	updateCells.setUniforms( shaderUniforms );
	updateCells.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	updateRender.setUniform2f( "resolution", (float)N_CELLS_X, (float)N_CELLS_Y );
	updateCells.setUniform2f( "screen", (float)width, (float)height );
	updateCells.setUniform1i( "mouseDown", mouseIsDown );
	updateCells.setUniform3f( "mousePos", mousePosition );


	// Draw cell texture to call shaders, logic happens in shaders
	cellPingPong.src->draw( 0, 0 );

	updateCells.end();

	cellPingPong.dst->end();
	cellPingPong.swap();

	// Rendering
	renderFBO.begin();
	ofClear( 0, 0, 0, 0 );
	updateRender.begin();
	updateRender.setUniforms( shaderUniforms );
	updateRender.setUniformTexture( "cellTex", cellPingPong.src->getTexture(), 0 );
	updateRender.setUniform2f( "resolution", (float)N_CELLS_X, (float)N_CELLS_Y );
	updateRender.setUniform2f( "screen", (float)width, (float)height );

	ofPushStyle();
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	ofSetColor( 255 );

	vboGrid.draw();

	ofDisableBlendMode();
	glEnd();

	updateRender.end();
	renderFBO.end();
	ofPopStyle();
}

void GameOfLifeScene::draw()
{
	ofBackground( 0 );

	//ofSetColor( 100, 255, 255 );

	cellPingPong.dst->draw( 0, 0, width / (10 - dataSrcSize), height / (10 - dataSrcSize) );

	ofPushStyle();

	ofEnableDepthTest();
	ofDisableAlphaBlending();

	camera.begin();
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	axisMesh.draw();

	instancedShader.begin();
	instancedShader.setUniforms( shaderUniforms );
	instancedShader.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	instancedShader.setUniform2f( "resolution", (float)N_CELLS_X, (float)N_CELLS_Y );
	instancedShader.setUniform2f( "screen", (float)width, (float)height );
	instancedShader.setUniform3f( "lightPos", lightPos );
	instancedShader.setUniform4f( "materialColor", materialColor );

	vboSphere.drawInstanced( OF_MESH_FILL, N_CELLS_X * N_CELLS_Y );

	glDisable( GL_CULL_FACE );
	instancedShader.end();

	ofDisableDepthTest();
	ofEnableAlphaBlending();

	ofPopStyle();

	//drawCoordinateSystem();
	//renderFBO.draw( 0, 0 );
	camera.end();

	ofSetColor( 255 );
	gui.draw();
}

void GameOfLifeScene::handleSphereRadiusChanged( float& val )
{
	ofSpherePrimitive sphere;
	sphere.set( cellSize, sphereResolution );
	vboSphere = sphere.getMesh();
}

void GameOfLifeScene::handleSphereResolutionChanged( int& val )
{
	ofSpherePrimitive sphere;
	sphere.set( cellSize, sphereResolution );
	vboSphere = sphere.getMesh();
}

void GameOfLifeScene::keyPressed( int key ) {

	// std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
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
	mousePosition.set( x, y, 0.0 );
}

void GameOfLifeScene::mouseReleased( int x, int y, int button )
{
	mouseIsDown = false;
}

void GameOfLifeScene::mouseDragged( int x, int y, int button )
{
	if (mouseIsDown)
		mousePosition.set( x, y, 0.0 );
}