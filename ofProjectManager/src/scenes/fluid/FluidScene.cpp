#include "FluidScene.h"

FluidScene::FluidScene() : ccScene( "Fluid" ) {}

void FluidScene::setup()
{
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles.push_back( ofPoint( i, i, i ) );
	}
}

void FluidScene::update()
{
	for (ofPoint& p : particles) {
		if (p.x > ofGetWidth()) p.x = 0;
		if (p.y > ofGetHeight()) p.y = 0;
		if (p.z > ofGetHeight()) p.z = 0;

		
	}
}

void FluidScene::draw()
{
	camera.begin();

	float width = ofGetWidth();
	float height = ofGetHeight();
	float depth = height;

	ofDrawBox( width * 0.5f, height * 0.5f, depth * 0.5f, width, height, depth );

	for (ofPoint const& p : particles) {
		ofDrawCircle( p, 3 );
	}

	camera.end();
}
