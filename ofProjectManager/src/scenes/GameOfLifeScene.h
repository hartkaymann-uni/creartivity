#pragma once

#include "ofMain.h"
#include <ofxAppUtils.h>


const int N_CELLS_X = 102;
const int N_CELLS_Y = 77;
const unsigned short INVINCIBILITY_DURATION = 10;

class GameOfLifeScene : public ofxFadeScene {

public:
	GameOfLifeScene();
	void setup();
	void update();
	void draw();

	void mouseDragged( int x, int y, int button );

private:
	ofShader shader;
	bool current_generation[N_CELLS_X][N_CELLS_Y];
	unsigned short invincible[N_CELLS_X][N_CELLS_Y];

	int getNeighbourCount( int x, int y );
	void setRadius( int x, int y, int r, bool val );

};