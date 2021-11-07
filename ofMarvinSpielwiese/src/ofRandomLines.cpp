#include "ofRandomLines.h"
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

void ofRandomLines::keyPressed(int key) {
	//change between Fullscreen or Window		
	if (key == 'f') ofToggleFullscreen();
	if (key == 'd') drawCurvedPolyLine();
	if (key == 'p') frameRate++;
	if (key == 'l') frameRate--;
}

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


void ofRandomLines::drawRandomPolyLine() {
	myLine.addVertex(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
}

void ofRandomLines::drawRandomLines() {

	for (int i = 0; i < 100; i++)
	{
		myDrawCol.set(ofRandom(255), ofRandom(255), ofRandom(255));
		ofSetColor(myDrawCol);
		ofDrawLine(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	}
}

