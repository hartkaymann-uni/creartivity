#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	for (int i = 0; i < hubs.size(); i++)
	{
		if (hubs[i].isVisible())
		{
			hubs[i].update();
		}
		else
		{
			hubs.erase(hubs.begin()+i);
		}
	}

	
	while (hubs.size() < 128)
	{
		Hub newHub(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), hue);
		hubs.push_back(newHub);
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
	if (key == 'f') ofToggleFullscreen();
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	Hub newHub(mouseX, mouseY, hue);
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
Hub::Hub(int startX, int startY, int hue) {
	float directionSpeed = 1;
	position = glm::vec2(startX, startY);
	direction = glm::vec2(ofRandom(-directionSpeed, directionSpeed), ofRandom(-directionSpeed, directionSpeed));
	size = 20;
}

Hub::~Hub() {

}

bool Hub::isVisible() {
	int overlap = 20;
	return(position.x > -overlap &&
		position.x < ofGetWidth() + overlap &&
		position.y > -overlap &&
		position.y < ofGetHeight() + overlap &&
		size > 1);
}

void Hub::update() {
	float time = ofGetElapsedTimef();
	position += direction;
	size -= 0.01;
}

void Hub::draw() {
	ofDrawCircle(position,size);
}