/*
*  @author: Irene Santana Martin, Christine Schuller, Kay Hartmann, Cosmo Strattner, Marvin Esche, Franziska Streifert
* 
*  May 2022
*
*  This class contains basic app functionalities for the interaction.
*/

#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
/// <summary>
/// Initializing all the variables and methods
/// </summary>
void ofApp::setup() {
	ofSetFrameRate( 60 );
	ofSetVerticalSync( true );
	ofBackground( 0 );

	width = ofGetWidth();
	height = ofGetHeight();
	device.setLogLevel( OF_LOG_VERBOSE );
	bool setup = device.setup( 0 );
	tracker.setup( device );
	tracker.enableTrackingOutOfFrame( true );

	sender.setup( HOST, PORT );

	newUserListener = tracker.newUser.newListener( [this]( ofxNiTE2::User::Ref u ) {
		std::cout << "New user: " << u->getId() << std::endl;
		registerUser( u );
		printUsers();
		} );

	lostUserListener = tracker.lostUser.newListener( [this]( ofxNiTE2::User::Ref u ) {
		std::cout << "Lost user: " << u->getId() << std::endl;
		removeUser( u );
		printUsers();
		} );

	sendConnectionStarted();
}

//--------------------------------------------------------------
/// <summary>
///  This method continuously updates the the transmitted device data from the kinect and picture.
/// </summary>
void ofApp::update() {
	device.update();
	// How many users
	for (auto i = 0; i < tracker.getNumUser(); i++) {

		ofxNiTE2::User::Ref user = tracker.getUser( i );

		const ofxNiTE2::Joint& LHD = user->getJoint( NITE_JOINT_LEFT_HAND );
		const ofxNiTE2::Joint& RHD = user->getJoint( NITE_JOINT_RIGHT_HAND );

		float xl = LHD.getGlobalPosition().x;
		float yl = LHD.getGlobalPosition().y;

		float xr = RHD.getGlobalPosition().x;
		float yr = RHD.getGlobalPosition().y;

		// Calibration 
		if (xl < left)left = xl;
		if (xl > right)right = xl;
		if (yl < top)top = yl;
		if (yl > bottom)bottom = yl;

		if (xr < left)left = xr;
		if (xr > right)right = xr;
		if (yr < top)top = yr;
		if (yr > bottom)bottom = yr;

		int id = user->getId();
		// Position of left hand
		users[id].positionLeft.x = ofMap( xl,left, right, 0.f, 1.f, true );
		users[id].positionLeft.y = 1.f - ofMap( yl, top, bottom, 0.f, 1.f, true );

		// Position of right hand
		users[id].positionRight.x = ofMap( xr, left, right, 0.f, 1.f, true );
		users[id].positionRight.y = 1.f - ofMap( yr, top, bottom, 0.f, 1.f, true );

		ofApp::user& u = users[id];
		//printf( "user %i: Left:[ %.3f, %.3f] Right:[ %.3f, %.3f ] \n", i, u.positionLeft.x, u.positionLeft.y, u.positionRight.x, u.positionRight.y );
	};

	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		sendUser( it->first, it->second );
		it++;
	}
}

//--------------------------------------------------------------
/// <summary>
/// Captured Userdata is sent to ofProjectManager. 
/// </summary>
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
/// <summary>
/// This method draws the skeleton and ??
/// </summary>
void ofApp::draw() {
	depthPixels = tracker.getPixelsRef( 1000, 4000 );
	depthTexture.loadData( depthPixels );

	// Draw skeleton 2D
	ofSetColor( 255 );
	depthTexture.draw( 0, 0 );
	ofSetColor(ofColor::green);
	tracker.draw();

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
/// <summary>
/// This method distributes an ID to an user
/// The first user is assigned to ID 1
/// </summary>
void ofApp::registerUser( ofxNiTE2::User::Ref u )
{
	user newUser;
	int id = u->getId();
	newUser.id = id;
	users[id] = newUser;

	ofxOscMessage m;
	m.setAddress("/user/new/") ;
	m.addInt32Arg( id );
	sender.sendMessage( m );

}

//--------------------------------------------------------------
/// <summary>
/// If a person leaves the section of interaction, the skeleton can`t be tracked anymore
/// So the User will be removed
/// </summary>
void ofApp::removeUser( ofxNiTE2::User::Ref user )
{
	int id = user->getId();
	users.erase( id );

	ofxOscMessage m;
	m.setAddress( "/user/lost/" );
	m.addInt32Arg( id );
	sender.sendMessage( m );
}

//--------------------------------------------------------------
/// <summary>
/// Testing Method
/// It prints the tracked Users with the ID in the Console
/// </summary>
void ofApp::printUsers() {
	std::cout << "Users: [ ";
	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		std::cout << it->first << " ";
		it++;
	}
	std::cout << " ]" << std::endl;
}

//--------------------------------------------------------------
/// <summary>
/// This method starts the connection for OSC
/// </summary>
void ofApp::sendConnectionStarted() {
	
	ofxOscMessage m;
	m.setAddress( "/connection" );
	m.addStringArg( "on" );
	m.addStringArg( HOST );
	m.addInt32Arg( PORT );
	sender.sendMessage( m );
}