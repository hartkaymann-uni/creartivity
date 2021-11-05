#include "ofApp.h"

int width = 0;
ofRectangle myRect;
int myRectSize = 50;

ofRectangle circleArray[];

int yMovement = 1;
int xMovement = 1;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("OpenFramworks 'Windowtitel'");
	myRect.set(ofGetWidth() / 2 - myRectSize / 2, ofGetHeight() / 2 - myRectSize / 2, myRectSize, myRectSize);
	
}

//--------------------------------------------------------------
void ofApp::update(){
	if (collisionWithWall()) {
		cout << "bounce" << endl;
		ofDrawCircle(myRect.getCenter(), 50);
	}
}


bool ofApp::collisionWithWall() {
	bool collision = false;

	if (myRect.getTop() <= 0)
	{
		myRect.setY(0);
		yMovement = -yMovement;
		collision = true;
	}
	if (myRect.getBottom() >= ofGetHeight())
	{
		myRect.setY(ofGetHeight() - myRectSize);
		yMovement = -yMovement;
		collision = true;
	}

	if (myRect.getLeft() <= 0)
	{
		myRect.setX(0);
		xMovement = -xMovement;
		collision = true;
	}
	if (myRect.getRight() >= ofGetWidth())
	{
		myRect.setX(ofGetWidth() - myRectSize);
		xMovement = -xMovement;
		collision = true;
	}

	myRect.setPosition(myRect.getX() + xMovement, myRect.getY() + yMovement);
	return collision;
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	//ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, 50);

	ofDrawLine(myRect.getX()+ myRectSize/2, myRect.getY() + myRectSize / 2,ofGetWidth() / 2, ofGetHeight() / 2);

	ofDrawRectangle(myRect);



	if (width != int(ofGetWidth()))
	{
		width = ofGetWidth();
		cout << width << endl;
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//change between Fullscreen or Window		
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
