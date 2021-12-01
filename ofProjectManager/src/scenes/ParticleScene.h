#pragma once

#include "ofMain.h"
#include "ofxGui.h";
#include "ofxAppUtils.h"
#include "ofBufferObject.h"

class ParticleScene : public ofxFadeScene{


	struct Particle {
		glm::vec4 pos;
		glm::vec4 vel;
		ofFloatColor color;
	};

	public:
		ParticleScene();
		ParticleScene(int n_particles);
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

		ofShader compute;
		ofShader renderShader;
		vector<Particle> particles;
		ofBufferObject particlesBuffer, particlesBuffer2;
		ofEasyCam camera;
		ofVbo vbo;

		ofxPanel gui;

		ofVboMesh groundGrid;
		ofVboMesh m_VboMesh;

		ofParameterGroup m_ShaderUniforms;
		ofParameter<bool> m_DrawArrows;
		ofParameter<bool> m_DirAsColor;
		ofParameter<float> fps;
		ofParameter<float> m_MaxSpeed;
		ofParameter<float> m_NoiseShift;
		ofParameter<float> m_DeltaNoiseShift;
		ofParameter<bool> m_MousePressed;
		ofParameter<glm::vec3> m_MousePosition;
		
		static const int FIELD_X = 25;
		static const int FIELD_Y = 10;
		float noiseField[FIELD_X][FIELD_Y];
};