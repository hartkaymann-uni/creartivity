#include "SpiralScene.h"

void SpiralScene::setup()
{
	particleCount.set( "Particles", 100, 10, 1000 );
	spiralRadius.set( "Radius", 100.f, 10.f, 500.f );
	spiralSpeed.set( "Speed", 1.f, 0.f, 2.f );
	trailLength.set( "Trail", 100.f, 0.f, 255.f );
	jiggleAmplitude.set( "Jiggle Amplitude", 1, 0, 2.f );
	jiggleSpeed.set( "Jiggle Speed", 0.01f, 0.f, 0.1f );

	particleCount.addListener( this, &SpiralScene::createParticles );
	spiralRadius.addListener( this, &SpiralScene::allocateSpiralFbo );

	createParticles( (int)particleCount.get() );
	allocateSpiralFbo( (int)spiralRadius.get() );

	gui.add( spiralRadius );
	gui.add( particleCount );
	gui.add( spiralSpeed );
	gui.add( trailLength );
	gui.add( jiggleAmplitude );
	gui.add( jiggleSpeed );
}

void SpiralScene::createParticles( int& count ) {
	particles.resize( count );
	for (Particle& p : particles) {
		float r = sqrt( ofRandom( 1.0 ) );
		float theta = ofRandom( 1.0 ) * 2 * PI;

		float x = r * cos( theta );
		float y = r * sin( theta );

		p = { {x, y}, r, theta };
	}
}

void SpiralScene::allocateSpiralFbo( int& r ) {
	spiralFbo.allocate( r * 2, r * 2, GL_RGB16F );

	particleRadius = r * 0.01;
}

void SpiralScene::update()
{
	// Update time
	time = ofGetElapsedTimef();

	for (Particle& p : particles) {
		float x, y;
		// TODO: Map particle radius onto min and max radius of spiral, so no particles clump up in the middle
		float jiggle = (jiggleAmplitude * ofNoise( p.theta * time * jiggleSpeed )) - (jiggleAmplitude * 0.5);

		float r = (spiralRadius * p.r) + (spiralRadius * jiggle - spiralRadius / 2.f);

		x = r * cos( p.theta + (time * spiralSpeed) );
		y = r * sin( p.theta + (time * spiralSpeed) );

		p.pos.set( x, y );
	}

	// Draw onto FBO
	spiralFbo.begin();
	ofPushStyle();

	ofEnableAlphaBlending();
	ofFill();

	// Draw transparent rect for fading effect
	ofSetColor( 10, 0, 0, trailLength );
	ofDrawRectangle( ofVec3f( 0.f ), spiralFbo.getWidth(), spiralFbo.getHeight() );

	ofSetColor( 255 );
	ofVec2f center = ofVec2f( spiralFbo.getWidth() / 2 );
	for (Particle p : particles) {
		ofDrawCircle( center + p.pos, particleRadius );
	}

	ofPopStyle();
	spiralFbo.end();
}

void SpiralScene::draw()
{
	ofBackground( 0 );

	ofVec2f cursor( ofGetMouseX(), ofGetMouseY() );
	spiralFbo.draw( cursor - ofVec2f( spiralFbo.getWidth() / 2 ) );
}



