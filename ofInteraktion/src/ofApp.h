/*
*  @author: Irene Santana Martin, Christine Schuller, Kay Hartmann, Cosmo Strattner, Marvin Esche, Franziska Streifert
* 
*  May 2022
* 
*  This header contains the basic methods and variables for the interaction.
*/

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxNiTE2.h";
#include "ofxNI2.h"
#include "ofxOsc.h"

// The same Host and Port is used in ofProjectManager
#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp{

	// Represents single kinect skeleton user data 
	// Only information of both hands is stored 
	struct user {
		int id;
		ofVec2f positionLeft;
		ofVec2f positionRight;
	};

	public:
		void setup();
		void update();
		void sendUser(int id, user& user);
		void draw();
		void exit();

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

		// User detection
		void sendConnectionStarted();
		void registerUser(ofxNiTE2::User::Ref user);
		void removeUser(ofxNiTE2::User::Ref user);
		void printUsers();
};
