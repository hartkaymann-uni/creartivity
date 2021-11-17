#include "ofNNviaMesh.h"

//--------------------------------------------------------------
void ofNNviaMesh::setup() {
	ofSetWindowTitle("Nervensystem");
	ofBackground(10, 0, 0);
	ofSetFrameRate(60);
	mesh.setMode(OF_PRIMITIVE_LINES);
	conectionDistance = 10;
	ammoutOfPoints = 1024;
	//speed = 20;
}

//--------------------------------------------------------------
void ofNNviaMesh::update() {
	ofSetWindowTitle("Nervensystem " + ofToString(conectionDistance) + ", " + ofToString(speed));
	ofSeedRandom(30);
	mesh.clear();
	mesh.addVertex(ofVec3f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2, 0));
	for (int i = 1; i < ammoutOfPoints; i++)
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

			if (distance < conectionDistance)
			{
				mesh.addIndex(i);
				mesh.addIndex(j);

			}

		}

	}

}

void ofNNviaMesh::drawLightning(ofVec3f Pos1, ofVec3f Pos2, float displace) {
	/*
	ofSetColor(255);
	ofDrawLine(100, 100, 100, -100, -100, -100);

	if (true) ofDrawLine(Pos1, Pos2);
	else
	{
		float mid_x = (Pos2.x + Pos1.x) / 2.0;
		float mid_y = (Pos2.y + Pos1.y) / 2.0;
		float mid_z = (Pos2.z + Pos1.z) / 2.0;

		mid_x += (ofRandom(1) - 0.5) * displace;
		mid_y += (ofRandom(1) - 0.5) * displace;
		mid_z += (ofRandom(1) - 0.5) * displace;

		ofVec3f mid = ofVec3f(mid_x, mid_y, mid_z);

		drawLightning(Pos1, mid, displace / 2);
		drawLightning(Pos2, mid, displace / 2);
	}
	*/
}


//--------------------------------------------------------------
void ofNNviaMesh::draw() {
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	if (true)
	{
		for (int i = 0; i < mesh.getVertices().size(); i++)
		{
			ofSetColor(255);
			ofDrawSphere(mesh.getVertex(i), 3);
		}
	}

	drawLightning(ofVec3f(1, 1, 1), ofVec3f(1, 1, 1), 0);

	mesh.draw();

}

//--------------------------------------------------------------
void ofNNviaMesh::keyPressed(int key) {
	if (key == 'w')
	{
		conectionDistance += 1;
		speed += 0.00001;
	}
	if (key == 's')
	{
		if (conectionDistance >= 10)	conectionDistance -= 1;
		speed -= 0.00001;
	}
}