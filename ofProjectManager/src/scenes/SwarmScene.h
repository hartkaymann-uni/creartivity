#pragma once

#include "ofMain.h"
#include "ofBufferObject.h"

#include "ccScene.h"


class SwarmScene : public ccScene
{
public:
	SwarmScene();

	void setup();
	void update();
	void draw();
	void dirAsColorChanged(bool& dirAsColor);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	struct Particle {
		glm::vec4 pos;
		glm::vec4 vel;
		ofFloatColor color;
	};

	int particleAmount;
	ofShader compute, colorSplash, particleShader;
	vector<Particle> particles;
	ofBufferObject particlesBuffer, particlesBuffer2;
	GLuint vaoID;
	ofVbo vbo;
	glm::vec3 atractor1, atractor2, atractor3;

	ofParameter<float> attractionCoeff, cohesionCoeff, repulsionCoeff;
	ofParameter<float> maxSpeed;
	ofParameter<float> attractor1Force, attractor2Force, attractor3Force;
	ofParameterGroup shaderUniforms;
	ofParameter<float> fps;
	ofParameter<bool> dirAsColor;

private:
	//--------------------------------------------------------------
	// Controls

	int mouseX;
	int mouseY;
	int mouseXClick;
	int mouseYClick;
	bool isPressingMouse;

	void UpdateMousePos(int x, int y, string action = "default");
	void ColorSplash();
};
