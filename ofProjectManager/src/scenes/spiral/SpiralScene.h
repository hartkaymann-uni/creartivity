#pragma once
#include "ccScene.h"
class SpiralScene :
	public ccScene
{
public:
	SpiralScene() : ccScene( "Spiral" ) {};

	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );
	void mouseMoved( int x, int y );

	void createParticles( int count );
	void allocateSpiralFbo( int radius );

private:

	struct Particle {
		ofVec2f pos;
		float r;
		float theta;
		float rnd;
		ofColor col;
	};

	float time = 0.f;

	ofVec2f cursor;
	bool isCursorActive = true;

	float particleRadius = 0.f;
	vector<Particle> particles;

	ofFbo spiralFbo;
	ofVec2f spiralPosition;

	ofParameterGroup params;
	ofParameter<int> particleCount;
	ofParameter<int> spiralInner;
	ofParameter<int> spiralOuter;
	ofParameter<float> spiralSpeed;
	ofParameter<float> particleSpeed;
	ofParameter<float> trailLength;
	ofParameter<float> jiggleAmplitude;
	ofParameter<float> jiggleSpeed;

};

