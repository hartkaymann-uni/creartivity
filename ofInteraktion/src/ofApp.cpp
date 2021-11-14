#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect1.open();

	width = 640;
	height = 480;

	ofxKinectV2::Settings ksettings;
	ksettings.enableRGB = true;
	ksettings.enableIR = false;
	ksettings.enableDepth = true;
	ksettings.config.MinDepth = 0.5;
	ksettings.config.MaxDepth = 8.0;

	// trying 2D contourFinder out
	
	color.allocate(width, height);
	gray.allocate(width, height);
	background.allocate(width, height);
	difference.allocate(width, height);
	

}

//--------------------------------------------------------------
void ofApp::update(){
	kinect1.update();

	if (kinect1.isFrameNew()) {
		textureDEPTH.loadData(kinect1.getDepthPixels());
	}

	// trying 2D contourFinder out
	
	if (kinect1.isFrameNew()) {
		color.setFromPixels(kinect1.getRgbPixels());
		color.resize(width, height);
		gray = color;

		if (learn) {
			background = gray;
			learn = false;
		}

		// background-subtraction = Hintergrundsubtraktion
		difference.absDiff(background, gray);

		// binarisation = Binarisierung
		difference.threshold(threshold);

		contour.findContours(difference, 10, width * height, 10, true);
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	textureDEPTH.draw(width, 0, width, height);

	// trying 2D contourFinder out
	
	// we can draw the whole contour
	/*
	contour.draw(0, 0, width, height);
	*/
	// or, instead we can draw each blob individually,
	for (int i = 0; i < contour.nBlobs; i++) {
		contour.blobs[i].draw(0, 0);
	}
	
	color.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::exit(){
	kinect1.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// trying 2D contourFinder out
	
	switch (key) {
	case 'q':
		threshold++;
		printf("threshold = %d\n", threshold);
		break;

	case 'w':
		threshold--;
		printf("threshold = %d\n", threshold);
		break;

	default:
		break;
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
