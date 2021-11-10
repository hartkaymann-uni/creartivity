#include "GameOfLifeScene.h"

#include <random>

GameOfLifeScene::GameOfLifeScene() : ofxFadeScene( "GameOfLife" ) {
	setSingleSetup( false );
	setFade( 1000, 1000 );
	width = ofGetWindowWidth();
	height = ofGetWindowHeight();
	timeStep = 0.0;
}

void GameOfLifeScene::setup()
{

	// Load Shaders
	filesystem::path shader_path( "../../res/shader" );
	bool loadUpdateCells = updateCells.load( shader_path / "passthru.vert", shader_path / "gol.frag" );
	bool loadUpdateRender = updateRender.load( shader_path / "render.vert", shader_path / "render.frag", shader_path / "render.geom");
	bool loadBasicShader = basicShader.load( shader_path / "passthru.vert", shader_path / "render.frag" );

	// Make array of float pixels with cell data
	// Currently only R value is be used, cells can only either be true (R > .5) or false (R =< .5)
	vector<float> cells( N_CELLS_X * N_CELLS_Y * 3 );
	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {
			cells.push_back( 1.0 );
			cells.push_back( 0.0 );
			cells.push_back( 0.0 );
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

	mesh.setMode( OF_PRIMITIVE_POINTS );
	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {
			mesh.addVertex( { x * 10.0 + x, y * 10.0 + y, 0} );
			mesh.addTexCoord( { x, y } );
		}
	}

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
	updateCells.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	updateCells.setUniform1i( "resolutionX", N_CELLS_X );
	updateCells.setUniform1i( "resolutionY", N_CELLS_Y );
	updateCells.setUniform2f( "screen", (float)width, (float)height );
	updateCells.setUniform1f( "timestep", (float) timeStep+=0.05 );

	// Draw cell texture to call shaders, logic happens in shaders
	cellPingPong.src->draw( 0, 0 );

	updateCells.end();

	cellPingPong.dst->end();
	cellPingPong.swap();

	// Rendering
	renderFBO.begin();
	ofClear( 0, 0, 0, 0 );
	updateRender.begin();
	updateRender.setUniformTexture( "cellTex", cellPingPong.dst->getTexture(), 0 );
	updateRender.setUniform1i( "resolutionX", N_CELLS_X );
	updateRender.setUniform1i( "resolutionY", N_CELLS_Y );
	updateRender.setUniform1f( "size", 10.0 );
	updateRender.setUniform2f( "screen", (float)width, (float)height );

	ofPushStyle();
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	ofSetColor( 255 );

	mesh.draw();

	ofDisableBlendMode();
	glEnd();

	updateRender.end();
	renderFBO.end();
	ofPopStyle();
}

void GameOfLifeScene::draw()
{
	ofBackground( 0 );

	ofSetColor( 100, 255, 255 );

	//renderFBO.draw( 0, 0);
	cellPingPong.dst->draw( 0, 0, width, height );

	basicShader.begin();

	ofDrawRectangle( ofRectangle( 100, 100, 100, 100 ) );
	basicShader.end();

}

void GameOfLifeScene::mouseDragged( int x, int y, int button )
{
	const int MOUSE_DRAG_RADIUS = 5;

	if (x > 0 && x < ofGetWindowWidth()
		&& y > 0 && ofGetWindowHeight())
	{
		setRadius( x / 10, y / 10, MOUSE_DRAG_RADIUS, true );
		//std::cout << x << " " << y << std::endl;
	}
}

void GameOfLifeScene::setRadius( int x, int y, int r, bool val )
{
	for (int y_shift = -r; y_shift <= r; y_shift++) {
		for (int x_shift = -r; x_shift <= r; x_shift++) {
			if (x_shift * x_shift + y_shift * y_shift <= r * r)
			{
				int x_shifted = x + x_shift;
				int y_shifted = y + y_shift;

				if (x_shifted > 0
					&& x_shifted < N_CELLS_X
					&& y_shifted > 0
					&& y_shifted < N_CELLS_Y)
				{
					//current_generation[x_shifted * N_CELLS_Y + y_shifted].alive = true;
					//invincible[x_shifted * N_CELLS_Y + y_shifted] = INVINCIBILITY_DURATION;
				}
			}
		}
	}
}
