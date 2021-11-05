#pragma once

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect1.init();
	kinect1.open();
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect1.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	kinect1.draw(0, 0, 640, 480);
}

//--------------------------------------------------------------
void ofApp::exit(){
	kinect1.setCameraTiltAngle(0);
	kinect1.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case OF_KEY_UP:
			angle++;
			if (angle > 30) {
				angle = 30;
			}
			kinect1.setCameraTiltAngle(angle);
			break;
		case OF_KEY_DOWN:
			angle--;
			if (angle > -30) {
				angle = -30;
			}
			kinect1.setCameraTiltAngle(angle);
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
