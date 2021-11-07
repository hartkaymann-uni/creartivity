#include "ofBasicsExamples01.h"
/*
Titel:		 BasicsExamples
Description: 2st file of following a tutorial by Daniel Buzzo
Link:		 https://www.youtube.com/watch?v=2Jz0LE6ovvg&list=PL6QF0yo3Zj7DbN76C5-_6VCDF5CPBIz6l
*/

//--------------------------------------------------------------
void ofBasicsExamples01::setup() {
}

//--------------------------------------------------------------
void ofBasicsExamples01::update() {
}


//--------------------------------------------------------------
void ofBasicsExamples01::draw() {
	drawWaves();
}

void ofBasicsExamples01::drawWaves() {
	float time = ofGetElapsedTimef();

	for (int i = 100; i < ofGetHeight()-100; i+=5)
	{
		ofSetColor(255, 255, 255);
		ofDrawCircle(ofGetWidth() * 1 / 4 + 100 * sin(i * 0.01 + time), i, 100);

		ofDrawCircle(ofGetWidth() * 2 / 4 + 100 * sin(i * 0.01 + time), i, 50 + 40 * sin( i * 0.005 + time ));

		ofSetColor(	127 + 127 * sin(i * 0.014 + time),
					127 + 127 * sin(i * 0.01 + time),
					127 + 127 * sin(i * 0.012 + time));
		ofDrawCircle(ofGetWidth() * 3 / 4 + 100 * cos(i * 0.01 + time), i, 50 + 40 * cos(i * 0.005 + time));
	}
}

void ofBasicsExamples01::keyPressed(int key) {
	//change between Fullscreen or Window		
	if (key == 'f') ofToggleFullscreen();
}