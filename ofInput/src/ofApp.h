#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp {

public:
	struct user {
		int id;
		glm::vec2 left;
		glm::vec2 right;
	};

	std::map<int, ofColor> userColors = { {0, ofColor(255,0,0)}, {1, ofColor(0,255,0)} , {2, ofColor(255,0,255)} , {3, ofColor(255,255,0)} , {4, ofColor(0,255,255)} };

	void setup();
	void update();
	void draw();

	void mouseDragged(int x, int y, int button);
	void windowResized(int w, int h);


private:
	int width, height;
	ofxPanel gui;
	ofxButton add_button, remove_button;
	ofParameter<bool> isSequencerInControl;
	glm::vec2 lastMousePos;

	std::map<int, user> users;

	ofxOscSender sender;

	glm::vec2 mapped(glm::vec2 v);
	glm::vec2 unmapped(glm::vec2 v);

	void sendUser(int id, user& user);
	void registerUser(int id);
	void removeUser(int id);

	void printUsers();
	void sendConnectionStarted();

	void handleRegisterUser();
	void handleRemoveUser();

	void createUserPattern(int x, int y);
	ofColor getUserColor(int id);

	// #### Sequencer
	enum class SequenceName {
		RandomSmooth,
		RandomJiggle,
		RandomFast,
		Edging,
		OutOfBounds
	};

	struct ParameterSequence
	{
		float duration;
		SequenceName sequenceType;

		ParameterSequence() {
			duration = 5;
			sequenceType = SequenceName::RandomSmooth;
		}

		ParameterSequence(float newDuration, SequenceName newType) {
			duration = newDuration;
			sequenceType = newType;
		}
	};

	vector<ParameterSequence> sequences;
	ParameterSequence currentSequence;
	int currentSequenceIndex;
	float lastSequenceTime, nextSequenceTime;

	void UpdateSequence();
	void CheckForNextSequence();
	void SetSequence(ParameterSequence sequence);
	void StartSequence();
	void OnSequencerControlChange(bool& inControl);
	// ####

	// #### Movement
	std::map<int, glm::vec2> userTargets; // Positions you want users to move towards. Used in some specific movement functions.

	void AssignRandomEdges();
	void AssignRandomOutOfBounds();
	void ApplyPerlinMovement();
	void MoveToTarget();
	void MoveUserTowardsPoint(int id, glm::vec2 point);
};
