#include "ofRandomLines.h"
ofColor myDrawCol;

//--------------------------------------------------------------
void ofRandomLines::setup() {
	ofSetFrameRate(10);
}

//--------------------------------------------------------------
void ofRandomLines::update() {
	myLine.addVertex(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));

}


//--------------------------------------------------------------
void ofRandomLines::draw() {

	ofSetLineWidth(5);
	//Random PolyLine
	myDrawCol.set(255, 0, 0, 75);
	ofSetColor(myDrawCol);
	myLine.draw();
	


}

/*
void ofRandomLines::drawRandomLines() {

	ofSetLineWidth(5);
	// Random Lines
	for (int i = 0; i < 100; i++)
	{
		myDrawCol.set(ofRandom(255), ofRandom(255), ofRandom(255));
		ofSetColor(myDrawCol);
		ofDrawLine(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	}
}
*/