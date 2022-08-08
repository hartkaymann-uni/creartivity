#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ccUserManager.h"


/// <summary>
/// Implementation of an OSC receiver. Can handle registration, updating and deletion of users. User manager needs to be set before receiveMessages() is called.
/// </summary>
class ccReceiver
{
public:
	ccReceiver( string host = "localhost", int port = 12345 );

	/// <summary>
	/// Receive and handle user data. 
	/// </summary>
	void receiveMessages();

	inline string getConnectionStatus() const { return connectionStatusString; }

private:

	ofxOscReceiver receiver;

	string connectionStatusString;
};

