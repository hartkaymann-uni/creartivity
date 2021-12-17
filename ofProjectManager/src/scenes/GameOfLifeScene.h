#pragma once

#include "ccScene.h"

#include "ofxUbo.h"

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
	ofFbo   FBOs[2];    // Real addresses of ping/pong FBO�s
};

class GameOfLifeScene : public ccScene {

public:
	GameOfLifeScene( int cells_x = 102, int cells_y = 77 );
	void setup();
	void update();
	void draw();
	void reset();

	void keyPressed( int key );
	void keyReleased( int key );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseDragged( int x, int y, int button );
	void windowResized(int w, int h);

	void handleSphereResolutionChanged( int& sphereRes );
	void handleDimensionsChanged( ofVec2f& value );

private:
	int n_cells_x;
	int n_cells_y;

	float time;

	ofShader    updateCells;
	ofShader    instancedShader;
	ofShader    outlineShader;

	pingPongBuffer cellPingPong;

	ofVboMesh vboSphere;

	ofParameterGroup shaderUniforms;
	ofParameter<ofVec2f> dimensions;
	ofParameter<int> sphereResolution;
	ofParameter<float> sphereRadius;
	ofParameter<float> evolutionFactor;
	ofParameter<float> dataSrcSize;
	ofParameter<float> mouseRadius;
	ofParameter<float> mouseStrength;
	ofParameter<float> jiggleFactor;

	bool mouseIsDown;
	ofVec3f mousePosition;

	float cellOffset;

	void allocateCellBuffer(int rows, int cols);
	float calculateSphereRadius( ofVec2f dim );
};
