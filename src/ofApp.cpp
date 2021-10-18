#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup() {

	const int N_CELLS_X = 102;
	const int N_CELLS_Y = 76;

	bool cells[N_CELLS_X][N_CELLS_Y];
	for (int i = 0; i < N_CELLS_X; i++) {
		for (int k = 0; k < N_CELLS_Y; k++) {
			if (i * k % (i + k + 1) < 10) cells[i][k] = true;
		}
	}

	shader.load( "gameOfLife.vert", "gameOfLife.frag" );
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor( 255 );
	shader.begin();
	ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );
	shader.end();
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
