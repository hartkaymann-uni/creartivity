#pragma once
#include "ofMain.h"

#include "ccScene.h"

#include "ofxGui.h"

class ContourLinesScene :public ccScene
{
public:
	ContourLinesScene();
	void setup();
	void update();
	void draw();

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

	ofShader contourLineShader;

	ofMesh mesh;

	ofxPanel gui;
	ofParameterGroup shaderUniforms;
	ofParameter<float> speed;
	ofParameter<float> scale;
	ofParameter<float> amplitude;
	ofParameter<int> radius;

	int meshWidth, meshHeight;
	float time;
	int sbv;
	int count;

};