#include "FluidScene.h"

FluidScene::FluidScene() : ccScene( "Fluid" ) {}

void FluidScene::setup()
{
	for (int i = 0; i < NUM_PARTICLES; i++) {
		Particle p = {
			{ofRandom( ofGetWidth() ), ofRandom( ofGetHeight() )},
			{ofRandomf(), ofRandomf()},
			0.f
		};

		particles.push_back( p );
	}
}

void FluidScene::update()
{
	// TODO set dimensions of domain in setup
	glm::vec2 center = glm::vec2( ofGetWidth(), ofGetHeight() ) * 0.5f;

	for (Particle& p : particles) {
		if (p.pos.x > ofGetWidth() || p.pos.x < 0
			|| p.pos.y > ofGetHeight() || p.pos.x < 0) p.pos = center;

		p.pos += p.vel;
	}
}

void FluidScene::draw()
{
	camera.begin();

	float width = ofGetWidth();
	float height = ofGetHeight();

	ofDrawRectangle( width * 0.5f, height * 0.5f, width, height );

	for (Particle const& p : particles) {
		ofDrawCircle( p.pos, 3 );
	}

	camera.end();
}
