#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// 2D input contourFinder
	/*
	cam.setup(width, height);
	cam.videoSettings();

	color.allocate(width, height);
	gray.allocate(width, height);
	background.allocate(width, height);
	difference.allocate(width, height);
	*/

	// 3D input
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);

	device.setLogLevel(OF_LOG_NOTICE);
	device.setup(0);
	tracker.setup(device);
	tracker.enableTrackingOutOfFrame(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	// 2D input contourFinder
	/*
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
	*/

	// 3D input
	device.update();

	// how many users
	for (int u = 0; u < tracker.getNumUser(); u++) {
		cout << "userNumber: " + ofToString(u) << endl;
		// in this case: show position of joint from left hand of all users
		tracker.getUser(u)->getJoint(NITE_JOINT_LEFT_HAND).getPosition();
	};

}

//--------------------------------------------------------------
void ofApp::draw(){
	// 2D input contourFinder
	/*
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
	*/

	// 3D input
	depthPixels = tracker.getPixelsRef(1000, 4000);
	depthTexture.loadData(depthPixels);

		// draw skeleton in 2D
	ofSetColor(255);
	depthTexture.draw(0, 0);
	tracker.draw();

		// draw skeleton in 3D skeleton in 2D
	/*
	ofPushView();
	tracker.getOverlayCamera().begin(ofRectangle(0, 0, depthTexture.getWidth(), depthTexture.getHeight()));
	ofDrawAxis(100);
	tracker.draw3D();
	tracker.getOverlayCamera().end();
	ofPopView();
	*/

	ofDrawBitmapString("Tracker FPS: " + ofToString(tracker.getFrameRate()), 20, ofGetHeight() - 40);
	ofDrawBitmapString("Application FPS: " + ofToString(ofGetFrameRate()), 20, ofGetHeight() - 20);

}

//--------------------------------------------------------------
void ofApp::exit(){
	// 3D input
	tracker.exit();
	device.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// 2D input contourFinder
	/*
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
	*/

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
