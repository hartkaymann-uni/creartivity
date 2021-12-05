#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

class ccScene : public ofxScene
{
public:
	ccScene(std::string name = "Unnamed Scene");

	inline ofxOscReceiver* getReceiver() const { return receiver; }
	
	inline void setReceiver( ofxOscReceiver* r ) { receiver = r; }

protected:

	void receiveMessage();

	int width, height;
	
	ofxOscReceiver* receiver;
	float blobsXf = 0.f;
	float blobsYf = 0.f;
};

