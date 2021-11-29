#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxNI2.h"
#include "ofxNiTE2.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
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

		// 2D input contourFinder
		/*
		ofVideoGrabber cam;

		ofxCvColorImage color;
		ofxCvGrayscaleImage gray;
		ofxCvGrayscaleImage background;
		ofxCvGrayscaleImage difference;

		ofxCvContourFinder contour;

		// for difference, so we can either see more or less contours, threshold for face=80
		int threshold = 57;
		// find contours on-screen
		bool learn = true;
		*/

		// 3D input
		ofxNI2::Device device;
		ofxNiTE2::UserTracker tracker;
		ofPixels depthPixels;
		ofTexture depthTexture;

};
