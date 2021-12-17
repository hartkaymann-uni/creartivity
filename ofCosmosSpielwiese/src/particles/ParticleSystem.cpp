#include "ParticleSystem.h"

//--------------------------------------------------------------
void ParticleSystem::setup() {
	ofSetVerticalSync(true);

	int num = 10000;
	p.assign(num, demoParticle());
	currentMode = PARICLE_MODE_FLOW_1;

	currentModeStr = "5 - PARICLE_MODE_FLOW_1: Cosmo is testing stuff";

	resetParticles();
}

//--------------------------------------------------------------
void ParticleSystem::resetParticles() {

	//these are the attraction points used in the forth demo 
	attractPoints.clear();
	for (int i = 0; i < 4; i++) {
		attractPoints.push_back(glm::vec3(ofMap(i, 0, 4, 100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100), 0));
	}

	attractPointsWithMovement = attractPoints;

	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].setMode(currentMode);
		p[i].setAttractPoints(&attractPointsWithMovement);
		p[i].reset();
	}
}

//--------------------------------------------------------------
void ParticleSystem::update() {
	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].update();
	}

	//lets add a bit of movement to the attract points
	for (unsigned int i = 0; i < attractPointsWithMovement.size(); i++) {
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}
}

//--------------------------------------------------------------
void ParticleSystem::draw() {
	//ofBackgroundGradient(ofColor(60, 60, 60), ofColor(10, 10, 10));

	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].draw();
	}

	ofSetColor(190);
	if (currentMode == PARTICLE_MODE_NEAREST_POINTS) {
		for (unsigned int i = 0; i < attractPoints.size(); i++) {
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.", 10, 20);
}

//------------------------------------------------------------------
void ParticleSystem::ColorExplosion(ofColor color, glm::vec2 source) {
	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].ColorExplosion(color, source);
	}
}

//------------------------------------------------------------------
void ParticleSystem::Impulse(glm::vec2 direction, glm::vec2 source) {
	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].Impulse(direction, source);
	}
}

//--------------------------------------------------------------
void ParticleSystem::ChangeAttractionMode(int key) {
	if (key == '1') {
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
		AfterModeSwap();
		
	}
	if (key == '2') {
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
		AfterModeSwap();
	}
	if (key == '3') {
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
		AfterModeSwap();
	}
	if (key == '4') {
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
		resetParticles();
		AfterModeSwap();
	}
	if (key == '5') {
		currentMode = PARICLE_MODE_FLOW_1;
		currentModeStr = "5 - PARICLE_MODE_FLOW_1: Cosmo is testing stuff";
		resetParticles();
		AfterModeSwap();
	}

	

	if (key == ' ') {
		resetParticles();
	}
}

//--------------------------------------------------------------
void ParticleSystem::AfterModeSwap() {
	for (unsigned int i = 0; i < p.size(); i++) p[i].setMode(currentMode);
	ofClear(0, 255);
	ofSetBackgroundColor(ofColor(0, 0, 0));
}