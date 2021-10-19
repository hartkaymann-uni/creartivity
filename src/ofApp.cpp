#include "ofApp.h"

void ofApp::setup()
{
	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {
			if (x * y % (x + y + 1) < 10) current_generation[x][y] = true;
		}
	}

	// Filepath to shader directory (this comment is mopstly for testing thats why its so useless)
	filesystem::path shader_path( "../../res/shaders" );
	shader.load( shader_path/"gameOfLife.vert", shader_path/"gameOfLife.frag" );
}

void ofApp::update()
{
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	// Basic game of life logic
	bool next_generation[N_CELLS_X][N_CELLS_Y] = { false };
	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {
			int n_neighbours = getNeighbourCount( x, y );

			// Cell is initially alive
			if (current_generation[x][y])
			{
				// Alive cells with 2 or 3 neightbours live in the next generation
				if (n_neighbours == 2 || n_neighbours == 3) next_generation[x][y] = true;
			}
			else
			{
				// Dead cells with 3 neightbours live in the next generation
				if (n_neighbours == 3) next_generation[x][y] = true;
			}
		}
	}
	std::copy( &next_generation[0][0], &next_generation[0][0] + N_CELLS_X * N_CELLS_Y, &current_generation[0][0] );
}


int ofApp::getNeighbourCount( int x, int y )
{
	int count = 0;
	if (current_generation[x - 1][y - 1]) count++;	// top-left
	if (current_generation[  x  ][y - 1]) count++;	// top-middle
	if (current_generation[x + 1][y - 1]) count++;	// top-right
	if (current_generation[x - 1][  y  ]) count++;	// middle-left
	if (current_generation[x + 1][  y  ]) count++;	// middle-right
	if (current_generation[x - 1][y + 1]) count++;	// bottom-left
	if (current_generation[  x  ][y + 1]) count++;	// bottom-middle
	if (current_generation[x + 1][y + 1]) count++;	// bottom-right
	return count;
}

void ofApp::draw()
{
	ofClear( 255 );
	shader.begin();
	for (int x = 0; x < N_CELLS_X; x++) {
		for (int y = 0; y < N_CELLS_Y; y++) {
			if (current_generation[x][y]) {
				ofDrawRectangle( x * 10, y * 10, 10, 10 );
			}
		}
	}
	shader.end();
}

void ofApp::keyPressed( int key )
{

}

void ofApp::keyReleased( int key ) {

}

void ofApp::mouseMoved( int x, int y )
{

}

void ofApp::mouseDragged( int x, int y, int button )
{

}

void ofApp::mousePressed( int x, int y, int button )
{

}

void ofApp::mouseReleased( int x, int y, int button )
{

}

void ofApp::mouseEntered( int x, int y )
{

}

void ofApp::mouseExited( int x, int y )
{

}

void ofApp::windowResized( int w, int h )
{

}

void ofApp::gotMessage( ofMessage msg )
{

}

void ofApp::dragEvent( ofDragInfo dragInfo )
{

}
