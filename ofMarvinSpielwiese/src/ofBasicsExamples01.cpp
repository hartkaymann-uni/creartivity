#include "ofBasicsExamples01.h"
/*
Titel:		 BasicsExamples
Description: 2st file of following a tutorial by Daniel Buzzo
Link:		 https://www.youtube.com/watch?v=2Jz0LE6ovvg&list=PL6QF0yo3Zj7DbN76C5-_6VCDF5CPBIz6l
*/

//--------------------------------------------------------------
void ofBasicsExamples01::setup() {
	ofSetBackgroundColor(0);
	font.load("Arial",30);


}

//--------------------------------------------------------------
void ofBasicsExamples01::update() {
}


//--------------------------------------------------------------
void ofBasicsExamples01::draw() {
	if (0) drawWaves();
	if (1) drawClock();
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

void ofBasicsExamples01::drawClock() {
	ofSetFrameRate(1);
	ofSetColor(255, 0, 0);
	for (int i = 0; i < 360; i += 6) {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight()/2);
		ofRotateDeg(ofGetSeconds() * 6 - 90);
		ofRotateDeg(i);

		if (ofGetSeconds() < 10){
			font.drawString(": 0" + ofToString(ofGetSeconds()), 370, 0);
		}else {
			font.drawString(": "  + ofToString(ofGetSeconds()), 370, 0);
		}

		
		ofSetColor(250, 250, 250, i - 50);
		ofPopMatrix();
	}	

	ofSetColor(0, 255, 0);

	for (int i = 0; i < 360; i += 6) {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofRotateDeg(ofGetMinutes() * 6 - 90);
		ofRotateDeg(i);

		if (ofGetMinutes() < 10) {
			font.drawString(": 0" + ofToString(ofGetMinutes()), 300, 0);
		}
		else {
			font.drawString(": " + ofToString(ofGetMinutes()), 300, 0);
		}

		ofSetColor(250, 250, 250, i - 50);
		ofPopMatrix();
	}

	ofSetColor(0, 0, 255);
	for (int i = 0; i < 360; i += 30) {
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofRotateDeg(ofGetHours() * 30 - 90);
		ofRotateDeg(i);
		font.drawString(ofToString(ofGetHours()), 250, 0);
		ofSetColor(250, 250, 250, i - 50);
		ofPopMatrix();
	}

}



void ofBasicsExamples01::keyPressed(int key) {
	//change between Fullscreen or Window		
	if (key == 'f') ofToggleFullscreen();
}