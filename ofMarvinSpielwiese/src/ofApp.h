#pragma once

#include "ofMain.h"

class Hub {

public:
	glm::vec2 force;
	glm::vec2 position;
	glm::vec2 direction;

	float size;

	bool isVisible();

	void update();
	void draw();

	Hub(int x, int y, int hue);		//construktor
	~Hub();							//destruktor

};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		vector<Hub> hubs;
		int hue;
	
};


