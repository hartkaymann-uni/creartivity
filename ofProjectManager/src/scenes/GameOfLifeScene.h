#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxUbo.h"
#include "ofxGui.h"

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

class GameOfLifeScene : public ofxFadeScene {

public:
	GameOfLifeScene( int cells_x = 102, int cells_y = 77 );
	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseDragged( int x, int y, int button );

	void handleSphereResolutionChanged( int& sphereRes);
	void handleSphereRadiusChanged( float& cellSize );

private:
	ofShader    updateCells;
	ofShader    updateRender;
	ofShader    instancedShader;

	pingPongBuffer cellPingPong;

	ofFbo   renderFBO;

	ofVboMesh vboGrid;
	ofVboMesh vboSphere;
	ofMesh axisMesh;

	ofEasyCam camera;

	ofLight light;
	ofVec3f lightPos;
	ofColor materialColor;

	ofxPanel gui;
	ofParameterGroup shaderUniforms;
	ofParameter<int> sphereResolution;
	ofParameter<float> sphereRadius;
	ofParameter<float> evolutionFactor;
	ofParameter<float> cellSize;
	ofParameter<float> dataSrcSize;
	ofParameter<float> mouseRadius;
	ofParameter<float> mouseStrength;
	ofParameter<float> jiggleFactor;

	const int N_CELLS_X;
	const int N_CELLS_Y;

	int width, height;
	float time;

	bool mouseIsDown;
	ofVec3f mousePosition;

};
