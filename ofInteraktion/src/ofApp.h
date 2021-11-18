#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxKinectV2.h"

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

		/*
		ofxKinectV2 kinect1;
		ofTexture texture;
		*/

		int width;
		int height;

		// trying 2D contourFinder out
		
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
		

};
