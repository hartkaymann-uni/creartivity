#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	filesystem::path res_path( "../../res/" );
	compute.setupShaderFromFile( GL_COMPUTE_SHADER, res_path / "shader/particle.comp" );
	compute.linkProgram();
	camera.setFarClip( ofGetWidth() * 10 );
	particles.resize( 1024 * 8 );

	int i = 0;
	for (auto& p : particles)
	{
		p.pos.x = ofRandom( -ofGetWidth() * 0.5, ofGetWidth() * 0.5 );
		p.pos.y = ofRandom( -ofGetHeight() * 0.5, ofGetHeight() * 0.5 );
		p.pos.z = 0;
		p.pos.w = 1;
		p.vel = { 0,0,0,0 };
		i++;
	}
	particlesBuffer.allocate( particles, GL_DYNAMIC_DRAW );
	particlesBuffer2.allocate( particles, GL_DYNAMIC_DRAW );

	vbo.setVertexBuffer( particlesBuffer, 4, sizeof( Particle ) );
	vbo.setColorBuffer( particlesBuffer, sizeof( Particle ), sizeof( glm::vec4 ) * 2 );
	vbo.disableColors();
	dirAsColor = true;

	ofBackground( 0 );
	ofEnableBlendMode( OF_BLENDMODE_ADD );

	gui.setup();
	shaderUniforms.setName( "Shader Parameters" );
	shaderUniforms.add( maxSpeed.set( "u_MaxSpeed", 2500, 0, 5000 ) );
	gui.add( shaderUniforms );
	gui.add( drawArrows.set( "Draw Arrows", false ) );
	gui.add( deltaNoiseShift.set( "Noise Shift", 0.001, 0.0, 1.0 ) );
	gui.add( fps.set( "fps", 60, 0, 60 ) );

	particlesBuffer.bindBase( GL_SHADER_STORAGE_BUFFER, 0 );
	particlesBuffer2.bindBase( GL_SHADER_STORAGE_BUFFER, 1 );

	noiseShift = 0;
	for (int x = 0; x < FIELD_X; x++) {
		for (int y = 0; y < FIELD_Y; y++) {
			noiseField[x][y] = ofNoise( glm::vec3( x, y, noiseShift ) );
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	fps = ofGetFrameRate();

	compute.begin();
	compute.setUniforms( shaderUniforms );
	compute.setUniform1f( "u_TimeLastFrame", ofGetLastFrameTime() );
	compute.setUniform1f( "u_ElapsedTime", ofGetElapsedTimef() );
	compute.setUniform1f( "u_ScreenWidth", ofGetScreenWidth() );
	compute.setUniform1f( "u_ScreenHeight", ofGetScreenHeight() );
	compute.setUniform1f( "u_NoiseShift", noiseShift );

	compute.dispatchCompute( (particles.size() + 1024 - 1) / 1024, 1, 1 );
	compute.end();

	particlesBuffer.copyTo( particlesBuffer2 );

	noiseShift += deltaNoiseShift;
	for (int x = 0; x < FIELD_X; x++) {
		for (int y = 0; y < FIELD_Y; y++) {
			noiseField[x][y] = ofNoise( glm::vec3( x, y, noiseShift ) );
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	camera.begin();
	ofSetColor( 255, 70 );
	glPointSize( 2 );
	vbo.draw( GL_POINTS, 0, particles.size() );

	//ofNoFill();
	//ofDrawBox( 0, 0, -ofGetHeight() * 2, ofGetWidth() * 4, ofGetHeight() * 4, ofGetHeight() * 4 );

	if (drawArrows)
	{
		int width = ofGetScreenWidth();
		int height = ofGetScreenHeight();
		int left = -ofGetScreenWidth() * 0.5;
		int bot = -ofGetScreenHeight() * 0.5;
		int arrowLength = 25;
		ofSetColor( ofColor::red );
		for (int x = 0; x < FIELD_X; x++) {
			for (int y = 0; y < FIELD_Y; y++) {
				auto xPos = left + (width / FIELD_X) * x;
				auto yPos = bot + (height / FIELD_Y) * y;
				ofDrawArrow( glm::vec3( xPos, yPos, 10.0 ), glm::vec3( xPos + sin( noiseField[x][y] * (2 * PI) ) * arrowLength, yPos + cos( noiseField[x][y] * (2 * PI) ) * arrowLength, 10.0 ), arrowLength / 10 );
			}
		}
	}

	camera.end();

	ofEnableBlendMode( OF_BLENDMODE_ALPHA );
	ofSetColor( 255 );
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key ) {

}

//--------------------------------------------------------------
void ofApp::keyReleased( int key ) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button ) {

}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button ) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button ) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h ) {

}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg ) {

}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo ) {

}
