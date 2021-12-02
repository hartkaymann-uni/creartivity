#pragma once

#include "ofMain.h"
#include "ofxAppUtils.h"

#include "ofxUbo.h"

class Node {

public:
	glm::vec2 boxMid;
	glm::vec2 position;
	glm::vec2 direction;

	void update(int i);
	void draw();

	Node(int x, int y);		//construktor
	~Node();				//destruktor

};

class WorleyScene : public ofxFadeScene {

public:
	WorleyScene();
	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseEntered( int x, int y );
	void mouseExited( int x, int y );
	void windowResized( int w, int h );
	void dragEvent( ofDragInfo dragInfo );
	void gotMessage( ofMessage msg );

private:

	ofEasyCam camera;
	ofVec2f positions[88];
	vector<Node> nodes;
	ofShader worleyShader;

	int width, height;

	
};
