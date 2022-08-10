#pragma once

#include "ofMain.h"
#include "ofBufferObject.h"

#include "ccScene.h"

#define MAX_SWARM_HANDS 30
#define ORIGINAL_MIN_PARTICLE_DEPTH 0
#define ORIGINAL_MAX_PARTICLE_DEPTH 10000

class SwarmScene : public ccScene
{
public:
	SwarmScene();

	enum class SequenceName {
		BlackHole,
		Explosion,
		NormalAttraction,
		BrainNeurons,
		Swarm,
		RepulsionStutter,
		Intro,
		Outro,
		BrainNeuronsCoarse,
		BrainNeuronsDense,
		BrainNeuronsFlashLight,
		CrazyTestOne,
		VeryClose,
		VeryDense,
		CrazyClose
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
		glm::vec4 vel;
		ofFloatColor color;
		glm::vec4 initialPos;
		glm::vec4 unique;
	};

	struct SortByDepthOperator
	{
		bool operator() (Particle const& L, Particle const& R) { return L.pos.z < R.pos.z; }
	};

	int particleGroups;
	int particleAmount;
	int maxParticleDepth;
	int minParticleDepth;
	vector<Particle> particles;
	ofBufferObject particlesBuffer, particlesBuffer2, particlesBuffer3;
	GLuint vaoID;
	ofVbo vbo;

	void setup();
	void update();
	void draw();

	// ### Controls
	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

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
	ofParameter<bool> isSequencerInControl;

	// ### Transitions
	float SceneIntro() override;
	float SceneOutro() override;

private:
	// ### Drawing
	float user_circle_alpha;
	float user_circle_radius;

	void drawParticles();
	void drawUserCircles();

	// ### Shaders
	ofShader particleShader, userEnter, behaviorShader, interactionShader, userCircleShader;
	ofShader introShader, changeDepthShader;

	void applyParticleBehavior();
	void applyInteraction();
	void changeParticleDepth(float newMin, float newMax);
	void revertParticleDepthToOriginal();
	array<ofVec3f, MAX_SWARM_HANDS> getUserHandsArray(ccScene::CoordinateSystem system);

	// ### Controls
	ofVec3f mousePosition;
	ofVec3f mouseClickPosition;
	bool isPressingMouse;

	void updateMousePos(int x, int y, string action = "default");

	// ### Sequencer
	vector<ParameterSequence> sequences;
	ParameterSequence currentSequence;
	int currentSequenceIndex;
	float lastSequenceTime, nextSequenceTime;

	void initSequences();
	void updateSequence();
	void checkForNextSequence();
	void setSequence(ParameterSequence sequence);
	void startSequence();
	void activateRules();

	// ### Other
	vector<Particle> sortParticles();
};
