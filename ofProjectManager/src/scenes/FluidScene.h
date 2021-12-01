#pragma once
#include "ofMain.h"

#include "ofxAppUtils.h"

#include "quadtree.h"

class FluidScene : public ofxFadeScene
{
public:
	FluidScene();

	void setup();
	void update();
	void draw();

	void keyPressed( int key );
	void keyReleased( int key );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void mouseDragged( int x, int y, int button );

private:

	float kernel(float d);

	int width, height;
	float t, dt;

	float particleSize = 4.0;

	std::vector<quadtree::Particle> particles;
	quadtree::node root;
};

