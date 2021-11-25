#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel( OF_LOG_VERBOSE );

	ofxKinectV2 tmp;
	ofxKinectV2::KinectDeviceInfo deviceInfo = tmp.getDeviceList()[0];

	ofxKinectV2::Settings ksettings;
	ksettings.enableDepth = true;
	ksettings.enableDepthRegistration = true;
	ksettings.config.MinDepth = 0.5;
	ksettings.config.MinDepth = 1.0;

	kinect.open(deviceInfo.serial, ksettings);

	width = ofGetWidth();
	height = ofGetHeight();

	depthFbo.allocate( width, height, GL_RGB );
	depthFbo.begin();
	ofClear( 255 );
	depthFbo.end();
}

//--------------------------------------------------------------
void ofApp::update() {
	kinect.update();

	if (kinect.isFrameNew()) {
		texture.loadData( kinect.getPixels() );
		depthTexture.loadData( kinect.getDepthPixels() );
		
	}
	else {
		return;
	}

	depthFbo.begin();
	ofClear( 0, 0, 0, 0 );
	depthTexture.draw( 0, 0 );
	depthFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	//texture.draw( 0, 0 );
	depthFbo.draw( 0, 0 );
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.close();
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
	width = ofGetWidth();
	height = ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg ) {

}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo ) {

}
