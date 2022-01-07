#include "IntroScene.h"

IntroScene::IntroScene() : ccScene( "Intro" ) {

}

void IntroScene::setup()
{

	filesystem::path shader_path( "../../res/shader" );

	particles.resize( 10 );
	for (Particle &p : particles) {
		float r = radius * sqrt( ofRandom( 1.0 ) );
		float theta = ofRandom( 1.0 ) * 2 * PI;
		
		float x = r * cos( theta );
		float y = r * sin( theta );

		p = { {x, y}, r, theta };
	}

	gui.setup();

	// TODO: gui, radius etc parameters, alpha trails
}

void IntroScene::update()
{
	// Update time
	time = ofGetElapsedTimef();

	for (Particle &p : particles) {
		float x, y;
		
		float r = p.r + radius * ofNoise( p.theta * time / 100 );

		x = r * cos( p.theta + time );
		y = r * sin( p.theta + time );

		p.pos.set( x, y );
	}
}

void IntroScene::draw()
{
	ofBackground( 0 );

	camera.begin();

	ofFill();
	ofColor( 255 );
	ofVec2f cursor( ofGetMouseX(), height - ofGetMouseY() );

	for (Particle p : particles) {
		ofDrawCircle( cursor + p.pos, 2 );
	}

	ofPopStyle();

	camera.end();
}


