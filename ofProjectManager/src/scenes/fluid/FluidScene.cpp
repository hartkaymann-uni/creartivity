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
	for (ofPoint const& p : particles) {
	}
}

void FluidScene::draw()
{
	camera.begin();

	for (ofPoint const& p : particles) {
		ofDrawCircle( p, 3 );
	}

	camera.end();
}
