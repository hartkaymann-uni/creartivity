#include "SpiralScene.h"

void SpiralScene::setup()
{
	// TODO: make parameters that influence size dependent on window size and recalculate on window resize
	particleCount.set( "Particles", 1000, 10, 10000 );
	spiralInner.set( "Inner Radius", 10, 0, 250 );
	spiralOuter.set( "Outer Radius", 100, 10, 500 );
	spiralSpeed.set( "Tracking Speed", 10.f, 0.f, 20.f );
	particleSpeed.set( "Particle Speed", 0.5f, 0.f, 1.f );
	trailLength.set( "Trail", .8f, .0f, 1.f );
	jiggleAmplitude.set( "Jiggle Amplitude", .5f, 0.f, 1.f );
	jiggleSpeed.set( "Jiggle Speed", .5f, 0.f, 1.f );

	particleCount.addListener( this, &SpiralScene::createParticles );
	spiralOuter.addListener( this, &SpiralScene::allocateSpiralFbo );

	createParticles( (int)particleCount.get() );
	allocateSpiralFbo( (int)spiralOuter.get() );

	spiralPosition = { 0.5f * ofGetWidth(), 0.5f * ofGetHeight() };

	gui.add( particleCount );
	gui.add( spiralInner );
	gui.add( spiralOuter );
	gui.add( spiralSpeed );
	gui.add( particleSpeed );
	gui.add( trailLength );
	gui.add( jiggleAmplitude );
	gui.add( jiggleSpeed );

	ofHideCursor();
}

void SpiralScene::createParticles( int& count ) {
	particles.resize( count );
	for (Particle& p : particles) {
		float r = sqrt( ofRandom( 1.0 ) );
		float theta = ofRandom( 1.0 ) * 2 * PI;

		float x = r * cos( theta );
		float y = r * sin( theta );

		float rnd = ofRandom( 1.0f );

		ofColor col = ofColor( ofRandom( 0, 255 ), ofRandom( 0, 255 ), ofRandom( 0, 255 ), 100 );

		p = { {x, y}, r, theta, rnd, col };

	}
}

void SpiralScene::allocateSpiralFbo( int& r ) {
	spiralFbo.allocate( r * 2.5, r * 2.5, GL_RGB16F );

	particleRadius = r * 0.02;
}

void SpiralScene::update()
{
	// Update time
	time = ofGetElapsedTimef();

	for (Particle& p : particles) {
		float x, y;

		float jiggle = jiggleAmplitude * (ofNoise( p.rnd * time * jiggleSpeed ) - .5f);
		float r = p.r + jiggle;
		r = ofMap( r, 0.0f, 1.0f, spiralInner, spiralOuter, false );

		x = r * cos( p.theta + (time * particleSpeed) );
		y = r * sin( p.theta + (time * particleSpeed) );

		p.pos.set( x, y );
	}
	
	// Draw onto FBO
	spiralFbo.begin();
	ofPushStyle();

	ofEnableAlphaBlending();
	ofFill();

	// Draw transparent rect for fading effect
	ofSetColor( 0, 255 - (255 * trailLength) );
	ofDrawRectangle( ofVec3f( 0.f ), spiralFbo.getWidth(), spiralFbo.getHeight() );

	ofSetColor( 255 );
	ofVec2f center = ofVec2f( spiralFbo.getWidth() / 2 );
	for (Particle p : particles) {
		//ofSetColor( p.col );
		float radius = ofMap( p.r * p.r, 0.f, 1.f, particleRadius * .1f, particleRadius );
		ofDrawCircle( center + p.pos, radius );
	}

	ofPopStyle();
	spiralFbo.end();
}

void SpiralScene::draw()
{
	ofBackground( 0 );

	// Calculate new spiral position
	{
		ofVec2f diff = cursor - spiralPosition;

		if (diff.length() > spiralSpeed) {
			diff = diff.normalize() * spiralSpeed;
		}

		spiralPosition += diff;

	}

	spiralFbo.draw( spiralPosition - ofVec2f( spiralFbo.getWidth() / 2 ) );
}

void SpiralScene::keyPressed( int key )
{
	if (key == ofKey::OF_KEY_SHIFT) {
		ofShowCursor();
		isCursorActive = false;
	}

}

void SpiralScene::keyReleased( int key )
{
	if (key == ofKey::OF_KEY_SHIFT) {
		ofHideCursor();
		isCursorActive = true;
	}

}

void SpiralScene::mouseMoved( int x, int y )
{
	if (isCursorActive)
		cursor = { 1.f * x, 1.f * y };
}



