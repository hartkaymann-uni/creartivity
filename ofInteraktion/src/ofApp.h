/*
*  @author: Irene Santana Martin, Christine Schuller, Kay Hartmann, Cosmo Strattner, Marvin Esche, Franziska Streifert
*
*  May 2022
*
*  In this header are the basic methods and variables for the interaction declared/set.
*/

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxNiTE2.h";
#include "ofxNI2.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp {

	// Represents single kinect skeleton user data 
	// Only information of both hands is stored 
	struct user {
		int id;
		ofVec2f left;
		ofVec2f right;
	};

public:
	void setup();
	void update();
	void draw();
	void exit();

private:
	int width, height;
	int left, right, top, bottom;

	// Tracking data for the Kinect
	ofxNI2::Device device;
	ofxNiTE2::UserTracker tracker;
	ofPixels depthPixels;
	ofTexture depthTexture;

	std::map<int, user> users;

	// For the transmission of OSC
	ofxOscSender sender;
	ofEventListener newUserListener;
	ofEventListener lostUserListener;

	void sendConnectionStarted();
	void registerUser( ofxNiTE2::User::Ref user );
	void sendUser( int id, user& user );
	void removeUser( ofxNiTE2::User::Ref user );
	void printUsers();

	glm::vec2 mapped( glm::vec2 v );
	glm::vec2 unmapped( glm::vec2 v );

};
