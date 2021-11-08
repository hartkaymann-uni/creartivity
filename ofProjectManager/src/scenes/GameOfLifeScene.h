#pragma once

#include "ofMain.h"
#include <ofxAppUtils.h>


class GameOfLifeScene : public ofxFadeScene {

public:
	GameOfLifeScene();
	void setup();
	void update();
	void draw();

	void mouseDragged( int x, int y, int button );

private:

	const int N_CELLS_X = 102;
	const int N_CELLS_Y = 77;
	const unsigned short INVINCIBILITY_DURATION = 10;

	ofShader shader;
	bool* current_generation;
	unsigned short* invincible;

	int getNeighbourCount( int x, int y );
	void setRadius( int x, int y, int r, bool val );

};