#pragma once

#include "ofMain.h"

struct PingPong {
public:
	void allocate( glm::vec2 dim, int internalFormat = GL_RGBA ) {
		// Allocate
		for (int i = 0; i < 2; i++) {
			FBOs[i].allocate( dim.x, dim.y, internalFormat );
			FBOs[i].getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
			FBOs[i].getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
		}

		//Assign
		read = &FBOs[0];
		write = &FBOs[1];

		// Clean
		clear();
	}

	void swap() {
		std::swap( read, write );
	}

	void clear() {
		FBOs[0].begin();
		ofClear( 0 );
		FBOs[0].end();
		swap();
	}

	ofFbo& operator[]( int n ) { return FBOs[n]; }
	ofFbo* read;
	ofFbo* write;

private:
	ofFbo   FBOs[2];    // Real addresses of FBOs
};
