#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//kinect1.open();

	width = 1280;
	height = 720;

	// trying 2D contourFinder out
	
	cam.setup(width, height);
	cam.videoSettings();

	color.allocate(width, height);
	gray.allocate(width, height);
	background.allocate(width, height);
	difference.allocate(width, height);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	/*
	kinect1.update();

	if (kinect1.isFrameNew()) {
		texture.loadData(kinect1.getPixels());
	}
	*/

	// trying 2D contourFinder out
	
	cam.update();

	if (cam.isFrameNew()) {
		color.setFromPixels(cam.getPixels());
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
	//texture.draw(0, 0, width, height);

	// trying 2D contourFinder out
	
	// we can draw the whole contour
	// contour.draw(0, 0, -width, height);
	// or, instead we can draw each blob individually,
	ofPushMatrix();
	ofTranslate(cam.getWidth(), 0);
	ofScale(-1, 1);
	for (int i = 0; i < contour.nBlobs; i++) {
		contour.blobs[i].draw(0, 0);

		printf("x = %f, y = %f\n", contour.blobs[1].boundingRect.getCenter().x, contour.blobs[1].boundingRect.getCenter().y);
	}
	ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::exit(){
	//kinect1.close();
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
