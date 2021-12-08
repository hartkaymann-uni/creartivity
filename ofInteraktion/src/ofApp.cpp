#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
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
	device.update();

	// How many users
	for (int u = 0; u < tracker.getNumUser(); u++) {
		cout << "userNumber: " + ofToString(u) << endl;
		// Show position of joint from left and right hand of all users
		cout << "position of left hand: x= " + ofToString(tracker.getUser(u)->getJoint(NITE_JOINT_LEFT_HAND).getPosition().x) 
			+ ", y= " + ofToString(tracker.getUser(u)->getJoint(NITE_JOINT_LEFT_HAND).getPosition().y) << endl;
		cout << "position of right hand: x= " + ofToString(tracker.getUser(u)->getJoint(NITE_JOINT_RIGHT_HAND).getPosition().x)
			+ ", y= " + ofToString(tracker.getUser(u)->getJoint(NITE_JOINT_RIGHT_HAND).getPosition().y) << endl;

		// Position left hand
		users[u].positionLeft.x = tracker.getUser(u)->getJoint(NITE_JOINT_LEFT_HAND).getPosition().x;
		users[u].positionLeft.y = tracker.getUser(u)->getJoint(NITE_JOINT_LEFT_HAND).getPosition().y;

		// Position right hand
		users[u].positionRight.x = tracker.getUser(u)->getJoint(NITE_JOINT_RIGHT_HAND).getPosition().x;
		users[u].positionRight.y = tracker.getUser(u)->getJoint(NITE_JOINT_RIGHT_HAND).getPosition().y;
	};

	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		sendUser(it -> first, it -> second);
		it++;
	}
}

//--------------------------------------------------------------
void ofApp::sendUser(int id, user& user) {
	ofxOscMessage m;
	std::string addr = "/user/data/";
	addr += ofToString(id);
	m.setAddress(addr);

	// Position left hand
	m.addFloatArg(user.positionLeft.x);
	m.addFloatArg(user.positionLeft.y);

	// Position rigt hand
	m.addFloatArg(user.positionRight.x);
	m.addFloatArg(user.positionRight.y);

	sender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::draw(){
	depthPixels = tracker.getPixelsRef(1000, 4000);
	depthTexture.loadData(depthPixels);

	// Draw skeleton 2D
	ofSetColor(255);
	depthTexture.draw(0, 0);
	tracker.draw();
	
	// Draw skeleton 2D in 3D
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
	tracker.exit();
	device.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
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
