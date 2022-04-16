#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#define HOST "localhost"
#define PORT 12345

class ofApp : public ofBaseApp {

	struct user {
		int id;
		glm::vec2 left;
		glm::vec2 right;
	};

public:
	void setup();
	void update();
	void draw();

	void mouseDragged( int x, int y, int button );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );

	void windowResized( int w, int h );

	int width, height;
	ofxPanel gui;
	ofParameter<bool> p_add;
	ofParameter<bool> p_remove;

	std::map<int, user> users;

	ofxOscSender sender;

	glm::vec2 mapped(glm::vec2 v);
	glm::vec2 unmapped(glm::vec2 v);

	void sendUser( int id, user& user );
	void registerUser( int id );
	void removeUser( int id );

	void printUsers();
	void sendConnectionStarted();

	void handleRegisterUser( bool& b );
	void handleRemoveUser( bool& b );
};
