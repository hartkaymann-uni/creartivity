#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345


struct user {
	int id;
	ofVec2f positionLeft;
	ofVec2f positionRight;
};

class ccReceiver
{

public:
	ccReceiver( string host = "localhost", int port = 12345 );

	void receiveMessages();

	inline unique_ptr<map<int, user>> getUsers() const { return make_unique<map<int, user>>( users ); }; // This looks like shit but i hope this might possibly even work like a wee little bit
	inline string getConnectionStatus() const { return connectionStatusString; };

private:

	ofxOscReceiver receiver;
	string connectionStatusString = "Status: off";

	map<int, user> users;

};

