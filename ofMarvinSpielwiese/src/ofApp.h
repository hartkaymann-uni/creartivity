#pragma once

#include "ofMain.h"

class Node {

public:
	glm::vec2 force;
	glm::vec2 position;
	glm::vec2 direction;

	float size;

	bool isVisible();

	void update();
	void draw();

	Node(int x, int y);		//construktor
	~Node();				//destruktor

};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void drawLightning(glm::vec2 Pos1, glm::vec2 Pos2, float displace = 50);

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

private:

	vector<Node> nodes;
	bool isPaused;
	bool lineIsEnabled;
	bool lightningIsEnabled;
	int maxDistance;

};


