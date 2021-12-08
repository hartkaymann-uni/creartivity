#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate( 60 );
	ofSetVerticalSync( true );
	ofBackground( 0 );

	width = ofGetWidth();
	height = ofGetHeight();

	device.setLogLevel( OF_LOG_VERBOSE );
	device.setup( 0 );
	tracker.setup( device );
	tracker.enableTrackingOutOfFrame( true );

	sender.setup( HOST, PORT );

}

//--------------------------------------------------------------
void ofApp::update() {
	device.update();

	// How many users
	for (auto u = 0; u < tracker.getNumUser(); u++) {

		ofxNiTE2::User::Ref user = tracker.getUser( u );

		const ofxNiTE2::Joint& LHD = user->getJoint( NITE_JOINT_LEFT_HAND );
		const ofxNiTE2::Joint& RHD = user->getJoint( NITE_JOINT_RIGHT_HAND );

		float xl = LHD.getGlobalPosition().x;
		float yl = LHD.getGlobalPosition().y;

		float xr = RHD.getGlobalPosition().x;
		float yr = RHD.getGlobalPosition().y;

		printf( "user %i: Left:[ %.3f, %.3f] Right:[ %.3f, %.3f ] \n", u, xl, yl, xr, yr );

		// Position left hand
		users[u].positionLeft.x = ofMap( xl, -width / 2.f, width / 2.f, 0.f, 1.f, true );
		users[u].positionLeft.y = ofMap( yl, -height / 2.f, height / 2.f, 0.f, 1.f, true );

		// Position right hand
		users[u].positionRight.x = ofMap( xr, -width / 2.f, width / 2.f, 0.f, 1.f, true );
		users[u].positionRight.y = ofMap( yr, -height / 2.f, height / 2.f, 0.f, 1.f, true );
	};

	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		sendUser( it->first, it->second );
		it++;
	}
}

//--------------------------------------------------------------
void ofApp::sendUser( int id, user& user ) {
	ofxOscMessage m;
	std::string addr = "/user/data/";
	addr += ofToString( id );
	m.setAddress( addr );

	// Position left hand
	m.addFloatArg( user.positionLeft.x );
	m.addFloatArg( user.positionLeft.y );

	// Position rigt hand
	m.addFloatArg( user.positionRight.x );
	m.addFloatArg( user.positionRight.y );

	sender.sendMessage( m );
}

//--------------------------------------------------------------
void ofApp::draw() {
	depthPixels = tracker.getPixelsRef( 1000, 4000 );
	depthTexture.loadData( depthPixels );

	// Draw skeleton 2D
	ofSetColor( 255 );
	depthTexture.draw( 0, 0 );
	tracker.draw();

	// Draw skeleton 2D in 3D
	/*
	ofPushView();
	tracker.getOverlayCamera().begin(ofRectangle(0, 0, depthTexture.getWidth(), depthTexture.getHeight()));
	ofDrawAxis(100);
	tracker.draw3D();
	tracker.getOverlayCamera().end();
	ofPopView();
	*/

	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		ofFill();
		ofSetColor( ofColor::red );
		ofDrawCircle( it->second.positionLeft, 10 );
		ofDrawCircle( it->second.positionRight, 10 );
		it++;
	}

	ofSetColor( 255 );
	ofDrawBitmapString( "Tracker FPS: " + ofToString( tracker.getFrameRate() ), 20, ofGetHeight() - 40 );
	ofDrawBitmapString( "Application FPS: " + ofToString( ofGetFrameRate() ), 20, ofGetHeight() - 20 );
}

//--------------------------------------------------------------
void ofApp::exit() {
	tracker.exit();
	device.exit();
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
