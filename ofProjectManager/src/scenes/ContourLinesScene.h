#pragma once
#include "ofMain.h"
#include "ofxAppUtils.h"
#include "ofxGui.h"

class ContourLinesScene:public ofxScene
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

	ofEasyCam camera;

	ofMesh mesh;

	ofxPanel gui;
	ofParameterGroup shaderUniforms;
	ofParameter<float> speed;
	ofParameter<float> scale;
	ofParameter<float> amplitude;
	
	int width, height;
	int meshWidth, meshHeight;
	float time;
	int sbv;
	int count;

};

