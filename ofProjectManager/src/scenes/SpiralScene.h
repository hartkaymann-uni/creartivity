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

	void createParticles( int& count );
	void allocateSpiralFbo( int& radius );

private:

	struct Particle {
		ofVec2f pos;
		float r;
		float theta;
	};

	float time = 0.f;
	float particleRadius = 0.f;
	vector<Particle> particles;

	ofFbo spiralFbo;

	ofParameterGroup params;
	ofParameter<int> particleCount;
	ofParameter<int> spiralRadius;
	ofParameter<float> spiralSpeed;
	ofParameter<float> trailLength;
	ofParameter<float> jiggleAmplitude;
	ofParameter<float> jiggleSpeed;

};

