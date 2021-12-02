#pragma once

#include "ofMain.h"

class solver
{
public:
	solver();
	void reset( int particlesWidth, int particlesHeight, vector<vector<float>> particlePositions, ofVec3f gridSize, ofVec3f gridResolution, int particleDensity );
	void simulate( float timeStep );

private:
	int particlesWidth = 0;
	int particlesHeight = 0;

	int gridWidth = 0;
	int gridHeight = 0;
	int gridDepth = 0;

	int gridResolutionX = 0;
	int gridResolutionY = 0;
	int gridResolutionZ = 0;

	float particleDensity = 0;

	int velocityTextureWidth = 0;
	int velocityTextureHeight = 0;

	int scalarTextureWidth = 0;
	int scalarTextureHeight = 0;

	int frameNumber = 0;

	float flipness = 0.99f;

	ofBufferObject quadBuffer;
	ofVbo quadVertexBuffer;
	
	ofBufferObject particleTextureCoordinateBuffer;
	ofVbo particleVertexBuffer;
	
	ofFbo simulationFrameBuffer;
	

	ofTexture particlePositionTexture;
	ofTexture particlePositionTextureTemp;
	
	ofTexture particleVelocityTexture;
	ofTexture particleVelocityTextureTemp;

	ofTexture particleRandomTexture;

	// Simulation textures
	ofTexture velocityTexture;
	ofTexture tempVelocityTexture;
	ofTexture originalVelocityTexture;
	ofTexture weightTexture;

	ofTexture markerTexture; // marks fluid/air, 1 of fluid, 0 if air
	ofTexture divergenceTexture;
	ofTexture pressureTexture;
	ofTexture tempSimulationTexture;

	ofShader transferToGridProgram;
	ofShader normalizeGridProgram;
	ofShader markProgram;
	ofShader addForceProgram;
	ofShader enforceBoundariesProgram;
	ofShader extendVelocityProgram;
	ofShader transferToParticlesProgram;
	ofShader divergenceProgram;
	ofShader jacobiProgram;
	ofShader subtractProgram;
	ofShader advectProgram;
	ofShader copyProgram;
};

