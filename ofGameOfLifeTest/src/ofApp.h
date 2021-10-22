#pragma once

#include "ofMain.h"

const int N_CELLS_X = 102;
const int N_CELLS_Y = 76;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseEntered( int x, int y );
	void mouseExited( int x, int y );
	void windowResized( int w, int h );
	void dragEvent( ofDragInfo dragInfo );
	void gotMessage( ofMessage msg );

private:
	ofShader shader;
	bool current_generation[N_CELLS_X][N_CELLS_Y];

	int getNeighbourCount( int x, int y );

};
