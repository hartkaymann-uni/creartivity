#include "ofApp.h"

#include <random>

void ofApp::setup()
{
	width = ofGetWindowWidth();
	height = ofGetWindowHeight();

	std::fill_n( current_generation, N_CELLS_X * N_CELLS_Y, false );
	std::fill_n( invincible, N_CELLS_X * N_CELLS_Y, 0 );

	// Load Shaders
	filesystem::path shader_path( "../../res/shaders" );
	updateCells.load( shader_path / "gameOfLife.vert", shader_path / "gameOfLife.frag" );
	updateRender.load( shader_path / "render.vert", shader_path / "render.frag" );

	// Make array of float pixels with cell data
	// Currently only R value is be used, as cells can only either be true or false
	vector<float> pos( N_CELLS_X * N_CELLS_Y * 3 );
	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; x < N_CELLS_Y; y++) {
			int i = N_CELLS_X * y + x;

			pos[i * 3 + 0] = ofRandom( 1.0 );
			pos[i * 3 + 1] = 0.0;
			pos[i * 3 + 2] = 0.0;
		}
	}

	// Load data into the FBO's texture
	cellPingPong.allocate( N_CELLS_X, N_CELLS_Y, GL_RGB32F );
	cellPingPong.src->getTexture().loadData( pos.data(), N_CELLS_X, N_CELLS_Y, GL_RGB );
	cellPingPong.dst->getTexture().loadData( pos.data(), N_CELLS_X, N_CELLS_Y, GL_RGB );

	// Allocate the render FBO
	renderFBO.allocate( width, height, GL_RGB );
	renderFBO.begin();
	ofClear( 0, 0, 0, 255 );
	renderFBO.end();
}

void ofApp::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	// Basic game of life logic
	/* Cell empty_cell = {false, 0, {0, 0, 0}};
	std::fill_n( next_generation, N_CELLS_X * N_CELLS_Y, empty_cell );

	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {

			int n_neighbours = getNeighbourCount( x, y );

			// Cell is initially alive
			if (current_generation[x * N_CELLS_Y + y])
			{
				// Alive cells with 2 or 3 neightbours live in the next generation
				if (n_neighbours == 2 || n_neighbours == 3) next_generation[x * N_CELLS_Y + y] = true;
			}
			else
			{
				// Dead cells with 3 neightbours live in the next generation
				if (n_neighbours == 3) next_generation[x * N_CELLS_Y + y] = true;
			}

			if (invincible[x * N_CELLS_Y + y] > 0) {
				next_generation[x * N_CELLS_Y + y] = true;
				invincible[x * N_CELLS_Y + y]--;
			}
		}
	}
	std::copy( &next_generation[0], &next_generation[0] + N_CELLS_X * N_CELLS_Y, &current_generation[0] );
	*/

	cellPingPong.dst->begin();
	ofClear( 0 );
	updateCells.begin();
	updateCells.setUniformTexture( "cellData", cellPingPong.src->getTexture(), 0 );
	updateCells.setUniform1i( "resolutionX", N_CELLS_X );
	updateCells.setUniform1i( "resolutionY", N_CELLS_Y );
	updateCells.setUniform2f( "screen", (float)width, (float)height );
	updateCells.setUniform1f( "timestep", (float)timeStep);

	// Draw cell texture to call shaders, logic happens in shaders
	cellPingPong.src->draw(0, 0);

	cellPingPong.dst->end();
	cellPingPong.swap();
}


int ofApp::getNeighbourCount( int x, int y )
{
	int count = 0;

	if (x > 0 && y > 0) if (current_generation[(x - 1) * N_CELLS_Y + (y - 1)].alive) count++;					// top-left
	if (y > 0) if (current_generation[x * N_CELLS_Y + (y - 1)].alive) count++;									// top-middle
	if (x < N_CELLS_X && y > 0) if (current_generation[(x + 1) * N_CELLS_Y + (y - 1)].alive) count++;			// top-right
	if (x > 0) if (current_generation[(x - 1) * N_CELLS_Y + y].alive) count++;									// middle-left
	if (x < N_CELLS_X) if (current_generation[(x + 1) * N_CELLS_Y + y].alive) count++;							// middle-right
	if (x > 0 && y < N_CELLS_Y) if (current_generation[(x - 1) * N_CELLS_Y + (y + 1)].alive) count++;			// bottom-left
	if (y < N_CELLS_Y) if (current_generation[x * N_CELLS_Y + (y + 1)].alive) count++;							// bottom-middle
	if (x < N_CELLS_X && y < N_CELLS_Y) if (current_generation[(x + 1) * N_CELLS_Y + (y + 1)].alive) count++;	// bottom-right

	return count;
}

void ofApp::draw()
{
	ofClear( 255 );
	shader.begin();
	shader.setUniformBuffer( "u_Cells", current_generation );

	shader.tex

		for (int x = 0; x < N_CELLS_X; x++) {
			for (int y = 0; y < N_CELLS_Y; y++) {
				if (current_generation[x * N_CELLS_Y + y]) {
					ofDrawRectangle( x * 10, y * 10, 10, 10 );
				}
			}
		}
	shader.end();
}

void ofApp::mouseDragged( int x, int y, int button )
{
	const int MOUSE_DRAG_RADIUS = 5;

	if (x > 0 && x < ofGetWindowWidth()
		&& y > 0 && ofGetWindowHeight())
	{
		setRadius( x / 10, y / 10, MOUSE_DRAG_RADIUS, true );
		//std::cout << x << " " << y << std::endl;
	}
}

void ofApp::setRadius( int x, int y, int r, bool val )
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
					current_generation[x_shifted * N_CELLS_Y + y_shifted].alive = true;
					invincible[x_shifted * N_CELLS_Y + y_shifted] = INVINCIBILITY_DURATION;
				}
			}
		}
	}
}
