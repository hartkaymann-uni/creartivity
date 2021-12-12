#pragma once

#include "ofMain.h"
#include "particles/particle.h"
#include "helper.h"
#include "particles/ParticleSystem.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

private:
	// Shader
	ofShader particleShader;
	ofShader particleTrailShader;


	// Input
	int mouseX;
	int mouseY;
	int mouseXClick;
	int mouseYClick;
	bool isPressingMouse;
 
	void updateMousePos(int x, int y, string action = "default");

	// Particles
	void setUpParticles();
	void updateParticles();
	void drawParticles();
	vector<particle> particles;
	int hue;
	ParticleSystem particleSystem;

	// Abilities
	bool isChargingImpulse;

	// GUI
	ofxIntSlider trail;
	ofxPanel gui;
};