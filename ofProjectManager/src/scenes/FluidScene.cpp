#include "FluidScene.h"

FluidScene::FluidScene() : ofxFadeScene( "Fluid" ) {
	setSingleSetup( false );
	setFade( 1000, 1000 );
	ofSetFrameRate( 60 );
}

void FluidScene::setup()
{
	filesystem::path res_path( "../../res/" );

	particles.resize( 100 );
	int i = 0;
	for (auto& p : particles)
	{
		p.pos.x = ofRandom( 0, ofGetWidth() / 10 );
		p.pos.y = ofRandom( 0, ofGetHeight() / 10 );
		p.pos.z = 0;
		p.pos.w = 1;
		p.vel.x = ofRandom( 1 ) - 0.5;
		p.vel.y = ofRandom( 1 ) - 0.5;
		i++;
	}

	root = quadtree::node( 0, glm::vec2( 0.f ), glm::vec2( ofGetWidth(), ofGetHeight() ), &particles );

}

void FluidScene::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	for (auto& particle : particles) {
		particle.pos += particle.vel;

		// Kepp particles on screen
		if (particle.pos.x < particleSize) {
			particle.pos.x = particleSize;
			particle.vel.x *= -1.0;
		}
		else if (particle.pos.x > ofGetWidth() - particleSize) {
			particle.pos.x = ofGetWidth() - particleSize;
			particle.vel.x *= -1.0;
		}

		if (particle.pos.y > ofGetHeight() - particleSize) {
			particle.pos.y = ofGetHeight() - particleSize;
			particle.vel.y *= -1.0;
		}
		else if (particle.pos.y > ofGetHeight() - particleSize) {
			particle.pos.y = ofGetHeight() - particleSize;
			particle.vel.y *= -1.0;
		}
	}

	root.update();
}

void FluidScene::draw()
{
	ofBackground( 0 );

	for (const auto& particle : particles) {
		glm::vec2 pos = particle.pos;
		ofDrawCircle( pos.x, pos.y, particleSize );
	}

	root.draw();
}

void FluidScene::keyPressed( int key )
{
}

void FluidScene::keyReleased( int key )
{
}

void FluidScene::mousePressed( int x, int y, int button )
{
}

void FluidScene::mouseReleased( int x, int y, int button )
{
}

void FluidScene::mouseDragged( int x, int y, int button )
{
}
