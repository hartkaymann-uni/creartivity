#pragma once

#include "ofMain.h"
#include "ofBufferObject.h"
#include "ofxGui.h";

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
		
		struct Particle {
			glm::vec4 pos;
			glm::vec4 vel;
			ofFloatColor color;
		};

		ofShader compute;
		vector<Particle> particles;
		ofBufferObject particlesBuffer, particlesBuffer2;
		ofEasyCam camera;
		ofVbo vbo;

		ofxPanel gui;

		ofParameterGroup shaderUniforms;
		ofParameter<bool> drawArrows;
		ofParameter<bool> dirAsColor;
		ofParameter<float> fps;
		ofParameter<float> maxSpeed;
		ofParameter<float> noiseShift;
		ofParameter<float> deltaNoiseShift;
		
		static const int FIELD_X = 25;
		static const int FIELD_Y = 10;
		float noiseField[FIELD_X][FIELD_Y];
};
