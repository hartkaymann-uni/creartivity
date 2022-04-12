#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ccUserManager.h"

#define HOST "localhost"
#define PORT 12345

class ccReceiver
{
public:
	ccReceiver( string host = "localhost", int port = 12345 );

	void receiveMessages();

	inline string getConnectionStatus() const { return connectionStatusString; };

private:



	ofxOscReceiver receiver;
	string connectionStatusString = "Status: off";
};

