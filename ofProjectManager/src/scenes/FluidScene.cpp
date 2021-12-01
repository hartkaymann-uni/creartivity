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
		p.index = i;
		p.pos.x = ofRandom( 0, ofGetWidth() / 10 );
		p.pos.y = ofRandom( 0, ofGetHeight() / 10 );
		p.vel.x = ofRandom( 2 ) - 1.0;
		p.vel.y = ofRandom( 2 ) - 1.0;
		p.mass = .1;
		i++;
	}

	root = quadtree::node( 0, 0.f, 0.f, width, height, &particles );
	root.build();

	t = 0;
	dt = 0.01;
}

void FluidScene::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	root.update();

	float interactionRadius = 50;
	float speedLimit = 10;

	std::vector<size_t> neighbours;

	for (auto& curr : particles) {
		neighbours.clear();
		neighbours.shrink_to_fit();

		// Find all neighbouring particles
		for (const auto& other : particles) {
			if (curr.index == other.index)
				break;

			glm::vec2 diff = curr.pos - other.pos;
			float dist = glm::length( diff );
			if (dist < interactionRadius) {
				// Add to neighbour list
				neighbours.push_back( other.index );
			}
		}

		float p = 0.0f;
		quadtree::Particle* other;

		for (const auto i : neighbours) {
			other = &(particles.at( i ));
			// Calulate density for current particle


			// Calculate pressure for current particle
			p += other->mass * kernel( glm::length( curr.pos - other->pos ) );

			// Change acceleration according to density and pressure
			
		}



		glm::vec2 adv( 0.f );
		glm::vec2 diff( 0.f );

		float p = 0.0f;

		// Update velocity
		glm::vec2 acc = adv + diff;
		curr.vel += acc;
		curr.vel *= .99f;

		// Limit velocity
		glm::vec2 dir = glm::normalize( curr.vel );
		if (glm::length( curr.vel ) > speedLimit) {
			curr.vel = dir * speedLimit;
		}

		// Update position (move screen boundary here)
		curr.pos += curr.vel;


		// Keep particles on screen
		if (curr.pos.x < particleSize) {
			curr.pos.x = particleSize + 5;
			curr.vel.x *= -1.0;
		}
		else if (curr.pos.x > ofGetWidth() - particleSize) {
			curr.pos.x = ofGetWidth() - particleSize - 5;
			curr.vel.x *= -1.0;
		}

		if (curr.pos.y > ofGetHeight() - particleSize) {
			curr.pos.y = particleSize + 5;
			curr.vel.y *= -1.0;
		}
		else if (curr.pos.y > ofGetHeight() - particleSize) {
			curr.pos.y = ofGetHeight() - particleSize - 5;
			curr.vel.y *= -1.0;
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

float FluidScene::kernel( float r )
{
	float h = 50.0f;
	float d = (1 / std::pow( PI, 3 / 2 ) * (h * h * h)) * std::exp( (r * r) / (h * h) );

	return d;
}
