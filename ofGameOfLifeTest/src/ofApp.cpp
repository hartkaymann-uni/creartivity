#include "ofApp.h"

void ofApp::setup()
{
	Cell empty_cell = { false, 0, {0, 0, 0} };
	std::fill_n( current_generation, N_CELLS_X * N_CELLS_Y, empty_cell );
	std::fill_n( invincible, N_CELLS_X * N_CELLS_Y, 0 );
	
	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {
			if (x * y % (x + y + 1) < 10)
				current_generation[x * N_CELLS_Y + y].alive = true;
		}
	}

	filesystem::path shader_path( "../../res/shaders" );
	shader.load( shader_path / "gameOfLife.vert", shader_path / "gameOfLife.frag" );
}

void ofApp::update()
{
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	// Basic game of life logic
	Cell empty_cell = { false, 0, {0, 0, 0} };
	std::fill_n( next_generation, N_CELLS_X * N_CELLS_Y, empty_cell );

	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {

			int n_neighbours = getNeighbourCount( x, y );

			// Cell is initially alive
			if (current_generation[x * N_CELLS_Y + y].alive)
			{
				// Alive cells with 2 or 3 neightbours live in the next generation
				if (n_neighbours == 2 || n_neighbours == 3)
					next_generation[x * N_CELLS_Y + y].alive = true;
			}
			else
			{
				// Dead cells with 3 neightbours live in the next generation
				if (n_neighbours == 3)
					next_generation[x * N_CELLS_Y + y].alive = true;
			}

			if (invincible[x * N_CELLS_Y + y] > 0) {
				next_generation[x * N_CELLS_Y + y].alive = true;
				invincible[x * N_CELLS_Y + y]--;
			}
		}
	}
	std::copy( &next_generation[0], &next_generation[0] + N_CELLS_X * N_CELLS_Y, &current_generation[0] );

	
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
			if (current_generation[x * N_CELLS_Y + y].alive) {
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
