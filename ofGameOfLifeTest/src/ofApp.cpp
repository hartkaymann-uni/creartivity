#include "ofApp.h"

void ofApp::setup()
{
	width = ofGetWindowWidth();
	height = ofGetWindowHeight();

	std::fill_n( current_generation, N_CELLS_X * N_CELLS_Y, false );
	std::fill_n( invincible, N_CELLS_X * N_CELLS_Y, 0 );

	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {
			if (x * y % (x + y + 1) < 10)
				current_generation[x * N_CELLS_Y + y] = true;
		}
	}

	filesystem::path shader_path( "../../res/shaders" );
	shader.load( shader_path / "gameOfLife.vert", shader_path / "gameOfLife.frag" );

	// ping pong buffering
	updateCells.load( shader_path / "gameOfLife.vert", shader_path / "gameOfLife.frag" );
	updateRender.load( shader_path / "render.vert", shader_path / "render.frag" );

}

void ofApp::update()
{
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	// Basic game of life logic
	bool next_generation[N_CELLS_X * N_CELLS_Y] = { false };
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

	//glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
	//glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_back, 0 );
	//glViewport( 0, 0, N_CELLS_X, N_CELLS_Y );
	//glBindTexture( GL_TEXTURE_2D, tex_front );

	fbo.begin();
	shader.begin();

	ofClear( 255, 255, 255, 0 );
	fbo.attachTexture( tex, GL_RGBA, 0 );

	shader.end();
	fbo.end();
}


int ofApp::getNeighbourCount( int x, int y )
{
	int count = 0;

	if (x > 0 && y > 0) if (current_generation[(x - 1) * N_CELLS_Y + (y - 1)]) count++;					// top-left
	if (y > 0) if (current_generation[x * N_CELLS_Y + (y - 1)]) count++;								// top-middle
	if (x < N_CELLS_X && y > 0) if (current_generation[(x + 1) * N_CELLS_Y + (y - 1)]) count++;			// top-right
	if (x > 0) if (current_generation[(x - 1) * N_CELLS_Y + y]) count++;								// middle-left
	if (x < N_CELLS_X) if (current_generation[(x + 1) * N_CELLS_Y + y]) count++;						// middle-right
	if (x > 0 && y < N_CELLS_Y) if (current_generation[(x - 1) * N_CELLS_Y + (y + 1)]) count++;			// bottom-left
	if (y < N_CELLS_Y) if (current_generation[x * N_CELLS_Y + (y + 1)]) count++;						// bottom-middle
	if (x < N_CELLS_X && y < N_CELLS_Y) if (current_generation[(x + 1) * N_CELLS_Y + (y + 1)]) count++;	// bottom-right
	return count;
}

void ofApp::draw()
{
	ofClear( 255 );
	shader.begin();
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
		std::cout << x << " " << y << std::endl;
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
					current_generation[x_shifted * N_CELLS_Y + y_shifted] = true;
					invincible[x_shifted * N_CELLS_Y + y_shifted] = INVINCIBILITY_DURATION;
				}
			}
		}
	}
}
