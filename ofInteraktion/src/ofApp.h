#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxNiTE2.h";
#include "ofxNI2.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp{

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

		// Tracker
		ofxNI2::Device device;
		ofxNiTE2::UserTracker tracker;
		ofPixels depthPixels;
		ofTexture depthTexture;

		std::map<int, user> users;

		ofxOscSender sender;
		ofEventListener newUserListener;
		ofEventListener lostUserListener;

		void sendConnectionStarted();
		void registerUser(ofxNiTE2::User::Ref user);
		void removeUser(ofxNiTE2::User::Ref user);
		void printUsers();
};
