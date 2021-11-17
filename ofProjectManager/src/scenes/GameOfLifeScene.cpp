#include "GameOfLifeScene.h"

#include <random>

GameOfLifeScene::GameOfLifeScene() : GameOfLifeScene( 102, 77 ) {}

GameOfLifeScene::GameOfLifeScene( int cells_x, int cells_y )
	: ofxFadeScene( "GameOfLife" ),
	width( ofGetWindowWidth() ),
	height( ofGetWindowHeight() ),
	timeStep( 0.0 ),
	N_CELLS_X( cells_x ),
	N_CELLS_Y( cells_y )
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
	bool loadInstancedShader = instancedShader.load( shader_path / "renderInstanced.vert", shader_path / "renderInstanced.frag");

	camera.disableMouseInput();
	camera.setupPerspective();
	camera.setPosition( 0, 0, 500 );
	camera.setFarClip( ofGetWidth() * 10 );

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

	circleResolution.set( "circleRes", 10, 4, 20 );
	evolutionFactor.set( "evolutionFac", 0.0, 0.0, 1.0 );
	cellSize.set( "size", 10.0, 1.0, 10.0 );
	cellSize.addListener( this, &GameOfLifeScene::handleCellSizeChanged );
	
	dataSrcSize.set( "srcSize", 0, 0, 9 );

	gui.setup();
	shaderUniforms.setName( "Shader Parameters" );
	shaderUniforms.add( circleResolution );
	shaderUniforms.add( evolutionFactor );
	shaderUniforms.add( cellSize);
	shaderUniforms.add( dataSrcSize );
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
	updateCells.setUniform1f( "timestep", (float)timeStep += 0.05 );

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
	updateRender.setUniform2f( "resolution", (float) N_CELLS_X, (float) N_CELLS_Y);
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

	cellPingPong.dst->draw( 0, 0 , width / (10 - dataSrcSize ), height / (10 - dataSrcSize));

	camera.begin();
	axisMesh.draw();

	instancedShader.begin();
	instancedShader.setUniforms( shaderUniforms );
	instancedShader.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	instancedShader.setUniform2f( "resolution", (float)N_CELLS_X, (float)N_CELLS_Y );
	instancedShader.setUniform2f( "screen", (float)width, (float)height );

	vboSphere.drawInstanced( OF_MESH_FILL, N_CELLS_X * N_CELLS_Y );
	instancedShader.end();

	//drawCoordinateSystem();
	
	//renderFBO.draw( 0, 0 );
	camera.end();

	ofSetColor( 255 );
	gui.draw();
}


void GameOfLifeScene::mouseDragged( int x, int y, int button )
{

}

void GameOfLifeScene::handleCellSizeChanged(float &cellSize)
{
	ofSpherePrimitive sphere;
	sphere.set( cellSize, 10 );
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