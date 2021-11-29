#include "FluidScene.h"

FluidScene::FluidScene()
	: ofxFadeScene( "Fluid" ),
	width( ofGetWidth() ),
	height( ofGetHeight() )
{
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
		p.vel.x = ofRandom( 2 ) - 1.0;
		p.vel.y = ofRandom( 2 ) - 1.0;
		i++;
	}

	root = quadtree::node( 0, 0.f, 0.f, width, height, &particles );
	root.build();
}

void FluidScene::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	root.update();

	for (auto& particle : particles) {
		// Keep particles on screen
		if (particle.pos.x < particleSize) {
			particle.pos.x = particleSize + 5;
			particle.vel.x *= -1.0;
		}
		else if (particle.pos.x > ofGetWidth() - particleSize) {
			particle.pos.x = ofGetWidth() - particleSize - 5;
			particle.vel.x *= -1.0;
		}

		if (particle.pos.y > ofGetHeight() - particleSize) {
			particle.pos.y = particleSize + 5;
			particle.vel.y *= -1.0;
		}
		else if (particle.pos.y > ofGetHeight() - particleSize) {
			particle.pos.y = ofGetHeight() - particleSize - 5;
			particle.vel.y *= -1.0;
		}
	}

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
