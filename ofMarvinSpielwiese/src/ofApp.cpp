#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i < hubs.size(); i++)
	{
		hubs[i].update();
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	for (int i = 0; i < hubs.size(); i++)
	{
		hubs[i].draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	hub newHub(mouseX, mouseY, hue);
	hubs.push_back(newHub);
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

//--------------------------------------------------------------
hub::hub(int startX, int startY, int hue) {

	position = glm::vec2(startX, startY);
	direction = glm::vec2(ofRandom(-2.0, 2.0), ofRandom(-2.0, 2.0));
	size = 20;
}

hub::~hub() {

}

void hub::update() {
	position += direction;
}

void hub::draw() {
	ofDrawCircle(position,size);
}