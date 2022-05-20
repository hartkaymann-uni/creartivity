#pragma once

#include "ofMain.h"
#include "ofBufferObject.h"

#include "ccScene.h"

#define MAX_SWARM_HANDS 30

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

	enum class SequenceName {
		BlackHole,
		Explosion,
		NormalAttraction,
		BrainNeurons,
		Swarm,
		RepulsionStutter,
		Intro,
		Outro,
		BrainNeuronsCoarse
	};

	struct ParameterSequence
	{
		float duration;
		float modifier;
		SequenceName sequenceType;

		ParameterSequence() {
			duration = -1;
			modifier = -1;
			sequenceType = SequenceName::BlackHole;
		}

		ParameterSequence(float newDuration, SequenceName newType, float newModifier = 1) {
			duration = newDuration;
			modifier = std::max(0.0f, std::min(newModifier, 5.0f));
			sequenceType = newType;
		}
	};

	struct Particle {
		glm::vec4 pos;
		glm::vec4 u;
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
	int maxParticleDepth;
	vector<Particle> particles;
	ofBufferObject particlesBuffer, particlesBuffer2, particlesBuffer3;
	GLuint vaoID;
	ofVbo vbo;

	// ### Parameters & GUI
	ofParameter<float> attractionCoeff, cohesionCoeff, repulsionCoeff;
	ofParameter<bool> UseAttraction, UseCohesion, UseRepulsion;
	ofParameter<bool> freezeParticles;
	ofParameter<glm::vec3> particleColorStart, particleColorEnd;
	ofParameter<float> maxSpeed;
	ofParameter<float> attractorForce;
	ofParameter<int> ruleIterationMod;
	ofParameterGroup shaderUniforms;
	ofParameter<float> fps;
	ofParameter<bool> dirAsColor;
	ofParameter<bool> isSequencerInControl;

	// Transitions
	float SceneIntro() override;
	float SceneOutro() override;
private:
	// ### Drawing
	void DrawParticles();
	void DrawUserCircles();

	// ### Shaders
	ofShader compute, colorSplash, particleShader, userEnter, introShader, behaviorShader, interactionShader, userCircleShader;

	void ApplyBiggusShadus();
	void ApplyParticleRules();
	void ApplyInteraction();
	array<ofVec3f, MAX_SWARM_HANDS> GetUserHandsArray(ccScene::CoordinateSystem system);

	// ### Controls
	ofVec3f mousePosition;
	ofVec3f mouseClickPosition;
	bool isPressingMouse;

	void UpdateMousePos(int x, int y, string action = "default");
	void ColorSplash();
	void UserEnter();
	vector<Particle> SortParticles();

	// ### Sequencer
	void InitSequences();
	void UpdateSequence();
	void CheckForNextSequence();
	void SetSequence(ParameterSequence sequence);
	void StartSequence();
	void ActivateRules();


	vector<ParameterSequence> sequences;
	ParameterSequence currentSequence;
	int currentSequenceIndex;
	float lastSequenceTime, nextSequenceTime;
};
