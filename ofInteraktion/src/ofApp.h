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
		ofVec2f positionLeft;
		ofVec2f positionRight;
	};

	public:
		void setup();
		void update();
		void sendUser(int id, user& user);
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		int width, height;

		// Tracker
		ofxNI2::Device device;
		ofxNiTE2::UserTracker tracker;
		ofPixels depthPixels;
		ofTexture depthTexture;

		std::map<int, user> users;

		ofxOscSender sender;
};
