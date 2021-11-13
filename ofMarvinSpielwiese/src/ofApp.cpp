#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Nervensystem");
	ofBackground(10,0,0);
	ofSetFrameRate(0);
	mesh.setMode(OF_PRIMITIVE_LINES);
	conectionDistance = 75;
	ammoutOfPoints = 1024;
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSeedRandom(60);
	mesh.clear();
	for (int i = 0; i < ammoutOfPoints; i++)
	{
		ofVec3f positon = ofVec3f
		(
			ofMap(ofNoise(ofRandom(600), ofGetElapsedTimef() * 0.0006), 0, 1, -1000, 1000),
			ofMap(ofNoise(ofRandom(600), ofGetElapsedTimef() * 0.0006), 0, 1, -1000, 1000),
			ofMap(ofNoise(ofRandom(600), ofGetElapsedTimef() * 0.0006), 0, 1, -10, 10)
		);
		mesh.addVertex(positon);
		mesh.addColor(ofColor(0, 255, 255));
	}

	for (int i = 0; i < mesh.getVertices().size(); i++)
	{
		auto positon = mesh.getVertex(i);
		for (int j = 0; j < mesh.getVertices().size(); j++)
		{
			auto jdistance = mesh.getVertex(j);
			auto distance = glm::distance(positon, jdistance);

			if (distance < conectionDistance && distance > 5)
			{
				mesh.addIndex(i);
				mesh.addIndex(j);
			}

		}

	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	if (false)
	{
		for (int i = 0; i < mesh.getVertices().size(); i++)
		{
			ofSetColor(255);
			ofDrawSphere(mesh.getVertex(i), 3);
		}
	}

	mesh.draw();

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
