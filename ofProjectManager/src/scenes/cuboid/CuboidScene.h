#pragma once

#include "ccScene.h"

class CuboidScene : public ccScene
{
public:
	CuboidScene();

	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );

private:

	ofShader cuboidShader;

	ofVboMesh cube;

	ofFbo renderFbo;

	ofParameterGroup uniforms;
	ofParameter<ofVec2f> gridDimensions;
	ofParameter<ofVec3f> cubeDimensions;

};

