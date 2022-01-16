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

	enum class SequenceType {
		BlackHole,
		Explosion,
		NormalAttraction,
		BrainNeurons,
		Swarm,
		RepulsionStutter
	};

	struct ParameterSequence
	{
		float duration;
		float modifier;
		SequenceType sequence;

		ParameterSequence(float newDuration, SequenceType newType, float newModifier = 1) {
			duration = newDuration;
			modifier = std::max(0.0f, std::min(newModifier, 5.0f));
			sequence = newType;
		}
	};

	struct Particle {
		glm::vec4 pos;
		glm::vec4 vel;
		ofFloatColor color;
		glm::vec4 unique;
		glm::vec4 initialPos;
		glm::vec4 bufferPos;
	};

	struct SortByDepthOperator
	{
		bool operator() (Particle const& L, Particle const& R) { return L.pos.z < R.pos.z; }
	};

	int particleGroups;
	int particleAmount;
	ofShader compute, colorSplash, particleShader, userEnter;
	vector<Particle> particles;
	ofBufferObject particlesBuffer, particlesBuffer2, particlesBuffer3;
	GLuint vaoID;
	ofVbo vbo;
	glm::vec3 atractor;

	ofParameter<float> attractionCoeff, cohesionCoeff, repulsionCoeff;
	ofParameter<bool> UseAttraction, UseCohesion, UseRepulsion;
	ofParameter<bool> only2D;
	ofParameter<float> maxSpeed;
	ofParameter<float> attractorForce;
	ofParameter<int> ruleIterationMod;
	ofParameterGroup shaderUniforms;
	ofParameter<float> fps;
	ofParameter<bool> dirAsColor;


private:
	//--------------------------------------------------------------
	// Controls

	ofVec3f mousePosition;
	ofVec3f mouseClickPosition;
	bool isPressingMouse;

	void UpdateMousePos(int x, int y, string action = "default");
	void ColorSplash();
	void UserEnter();
	vector<Particle> SortParticles();

	//--------------------------------------------------------------
	// Sequence Stuff
	void InitSequences();
	void UpdateSequences();
	void StartSequence();
	void UpdateParameters();
	void ActivateRules();


	vector<ParameterSequence> sequences;
	float lastSequenceTime;
	float nextSequenceTime;
	int currentSequenceIndex;
	float currentSequenceMod;
	SequenceType currentSequenceType;
};
