#pragma once

#include "ofMain.h"
#include "ofxUbo.h"

const int N_CELLS_X = 102;
const int N_CELLS_Y = 77;
const unsigned short INVINCIBILITY_DURATION = 10;

struct pingPongBuffer {
public:
	void allocate( int _width, int _height, int _internalformat = GL_RGBA ) {
		// Allocate
		for (int i = 0; i < 2; i++) {
			FBOs[i].allocate( _width, _height, _internalformat );
			FBOs[i].getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
		}

		//Assign
		src = &FBOs[0];
		dst = &FBOs[1];

		// Clean
		clear();
	}

	void swap() {
		std::swap( src, dst );
	}

	void clear() {
		for (int i = 0; i < 2; i++) {
			FBOs[i].begin();
			ofClear( 0, 255 );
			FBOs[i].end();
		}
	}

	ofFbo& operator[]( int n ) { return FBOs[n]; }
	ofFbo* src;       // Source       ->  Ping
	ofFbo* dst;       // Destination  ->  Pong

private:
	ofFbo   FBOs[2];    // Real addresses of ping/pong FBO«s
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void mouseDragged( int x, int y, int button );

private:
	ofShader    updateCells;
	ofShader    updateRender;

	pingPongBuffer cellPingPong;
	
	ofFbo   renderFBO;

	float   timeStep;


	int width, height;

	bool current_generation[N_CELLS_X * N_CELLS_Y] = { false };
	unsigned short invincible[N_CELLS_X * N_CELLS_Y] = { 69};

	int getNeighbourCount( int x, int y );
	void setRadius( int x, int y, int r, bool val );

};
