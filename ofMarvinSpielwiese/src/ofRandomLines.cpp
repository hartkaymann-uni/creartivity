#include "ofRandomLines.h"
/*
Titel:		 RandomLines
Description: 1st file of following a tutorial by Daniel Buzzo
Link:		 https://www.youtube.com/watch?v=2Jz0LE6ovvg&list=PL6QF0yo3Zj7DbN76C5-_6VCDF5CPBIz6l
*/


ofColor myDrawCol;
int frameRate;

//--------------------------------------------------------------
void ofRandomLines::setup() {
	ofSetFrameRate(frameRate = 60);

	ofSetLineWidth(5);
	myDrawCol.set(255, 0, 0, 75);
	ofSetColor(myDrawCol);

	linePercent = 0.0;
}

//--------------------------------------------------------------
void ofRandomLines::update() {
	ofSetFrameRate(frameRate);
}


//--------------------------------------------------------------
void ofRandomLines::draw() {
	if (0) drawRandomPolyLine();
	if (0) drawRandomLines();
	myLine.draw();
	if (1) drawPointAtCurvedLine();
}

//--------------------------------------------------------------
void ofRandomLines::keyPressed(int key) {
	//change between Fullscreen or Window		
	if (key == 'f') ofToggleFullscreen();
	if (key == 'd') drawCurvedPolyLine();
	if (key == 'p') frameRate++;
	if (key == 'l') frameRate--;
}

//--- CurvedPolyLine --------------------------------------------------------------
void ofRandomLines::drawCurvedPolyLine() {
	myLine.clear();
	for (int i = 0; i < 10; i++) myLine.curveTo(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	myLine.close();
	linePercent = 0;
}

void ofRandomLines::drawPointAtCurvedLine() {
	linePercent += 0.005;
	ofDrawCircle(myLine.getPointAtPercent(linePercent), 20);
}

//--- RandomPolyLine --------------------------------------------------------------
void ofRandomLines::drawRandomPolyLine() {
	myLine.addVertex(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
}

//--- RandomLines --------------------------------------------------------------
void ofRandomLines::drawRandomLines() {

	for (int i = 0; i < 100; i++)
	{
		myDrawCol.set(ofRandom(255), ofRandom(255), ofRandom(255));
		ofSetColor(myDrawCol);
		ofDrawLine(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	}
}

