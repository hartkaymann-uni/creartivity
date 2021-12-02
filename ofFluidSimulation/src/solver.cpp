#include "solver.h"

solver::solver()
{
	ofVec2f quad[4] = { {-1.0f, -1.0f}, {-1.0f, 1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f} };
	quadVertexBuffer.setVertexData( &quad[0], 8, GL_STATIC_DRAW );

	filesystem::path shader_path( "../../res/shader" );

	transferToGridProgram.load( shader_path / "transfertrogrid" );
	normalizeGridProgram.load( shader_path / "fullscreen.vert", shader_path / "normalizegrid.frag" );
	markProgram.load( shader_path / "mark" );
	addForceProgram.load( shader_path / "fullscreen.vert", shader_path / "forceProgram.frag" );
	enforceBoundariesProgram.load( shader_path / "fullscreen.vert", shader_path / "enforceboundaries.frag" );
	extendVelocityProgram.load( shader_path / "fullscreen.vert", shader_path / "extendvelocity.frag" );
	transferToParticlesProgram.load( shader_path / "fullscreen.vert", shader_path / "transfertoparticles.frag" );
	divergenceProgram.load( shader_path / "fullscreen.vert", shader_path / "divergence.frag" );
	jacobiProgram.load( shader_path / "fullscreen.vert", shader_path / "jacobi.frag" );
	subtractProgram.load( shader_path / "fullscreen.vert", shader_path / "subtract.frag" );
	advectProgram.load( shader_path / "fullscreen.vert", shader_path / "advect.frag" );
	copyProgram.load( shader_path / "fullscreen.vert", shader_path / "copy.frag" );
}

void solver::reset( int _particlesWidth, int _particlesHeight, vector<vector<float>> _particlePositions, ofVec3f _gridSize, ofVec3f _gridResolution, int _particleDensity )
{
	particlesWidth = _particlesWidth;
	particlesHeight = _particlesHeight;

	gridWidth = _gridSize.x;
	gridHeight = _gridSize.y;
	gridDepth = _gridSize.z;

	gridResolutionX = _gridResolution.x;
	gridResolutionY = _gridResolution.y;
	gridResolutionZ = _gridResolution.z;

	particleDensity = _particleDensity;

	velocityTextureWidth = (gridResolutionX + 1) * (gridResolutionZ + 1);
	velocityTextureHeight = (gridResolutionY + 1);

	scalarTextureWidth = gridResolutionX * gridResolutionZ;
	scalarTextureHeight = gridResolutionY;

	// Create particle data
	int particleCount = particlesWidth * particlesHeight;

	vector <float> particleTextureCoordinates;
	particleTextureCoordinates.reserve( particleCount * 2 );
	for (size_t y = 0; y < particlesHeight; y++) {
		for (size_t x = 0; x < particlesWidth; x++) {
			particleTextureCoordinates[(y * particlesWidth + x) * 2] = (x + 0.5f) / particlesWidth;
			particleTextureCoordinates[(y * particlesWidth + x) * 2 + 1] = (y + 0.5f) / particlesHeight;
		}
	}

	particleTextureCoordinateBuffer.allocate( particleTextureCoordinates, GL_STATIC_DRAW );
	particleVertexBuffer.setVertexBuffer( particleTextureCoordinateBuffer, sizeof( float ), 2, sizeof( float ) );

	// Generate initial particle positions and create texture for them
	vector<float> particlePositionData;
	particlePositionData.reserve( particleCount * 4 );
	vector<float> particleRandoms;
	particleRandoms.reserve( particleCount * 4 );

	for (size_t i = 0; i < particleCount; i++) {
		particlePositionData[i * 4] = _particlePositions[i][0];
		particlePositionData[i * 4 + 1] = _particlePositions[i][1];
		particlePositionData[i * 4 + 2] = _particlePositions[i][2];
		particlePositionData[i * 4 + 3] = 0.0;

		float theta = ofRandom( 1.0f ) * 2.0 * PI;
		float u = ofRandom( 1.0f ) * 2.0 - 1.0;
		particleRandoms[i * 4] = sqrt( 1.0 - u * u ) + cos( theta );
		particleRandoms[i * 4 + 1] = sqrt( 1.0 - u * u ) + sin( theta );
		particleRandoms[i * 4 + 2] = u;
		particleRandoms[i * 4 + 3] = 0.0;
	}

	std::vector<float> empty;
	empty.resize( particleCount * 4 );

	particlePositionTexture.loadData( particlePositionData.data(), particlesWidth, particlesHeight, GL_RGBA ); // TODO set GL_CLAMP_TO_EDGE and GL_NEAREST
	particlePositionTexture.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	particlePositionTextureTemp.loadData( empty.data(), particlesWidth, particlesHeight, GL_RGBA ); // TODO initially set data to null
	particlePositionTextureTemp.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );

	particleVelocityTexture.loadData( empty.data(), particlesWidth, particlesHeight, GL_RGBA );
	particleVelocityTexture.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	particleVelocityTextureTemp.loadData( empty.data(), particlesWidth, particlesHeight, GL_RGBA );
	particleVelocityTextureTemp.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );

	particleRandomTexture.loadData( particleRandoms.data(), particlesWidth, particlesHeight, GL_RGBA );

	// Create Simulation textures
	velocityTexture.loadData( empty.data(), velocityTextureWidth, velocityTextureHeight, GL_RGBA );
	velocityTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
	tempVelocityTexture.loadData( empty.data(), velocityTextureWidth, velocityTextureHeight, GL_RGBA );
	tempVelocityTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
	originalVelocityTexture.loadData( empty.data(), velocityTextureWidth, velocityTextureHeight, GL_RGBA );
	originalVelocityTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
	weightTexture.loadData( empty.data(), velocityTextureWidth, velocityTextureHeight, GL_RGBA );
	weightTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );

	markerTexture.loadData( empty.data(), scalarTextureWidth, scalarTextureHeight, GL_RGBA );
	markerTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
	divergenceTexture.loadData( empty.data(), scalarTextureWidth, scalarTextureHeight, GL_RGBA );
	divergenceTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
	pressureTexture.loadData( empty.data(), scalarTextureWidth, scalarTextureHeight, GL_RGBA );
	pressureTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );
	tempSimulationTexture.loadData( empty.data(), scalarTextureWidth, scalarTextureHeight, GL_RGBA, );
	tempSimulationTexture.setTextureMinMagFilter( GL_LINEAR, GL_LINEAR );

	simulationFrameBuffer.allocate( velocityTextureWidth, velocityTextureHeight ); // RGB?
}

void solver::simulate( float timeStep ) {
	if (timeStep == 0.0) return;

	frameNumber += 1;

	/*
		the simulation process
		transfer particle velocities to velocity grid
		save this velocity grid

		solve velocity grid for non divergence

		update particle velocities with new velocity grid
		advect particles through the grid velocity field
	*/

	// transfer particle velocities to grid
	simulationFrameBuffer.begin();
	transferToGridProgram.begin();

	transferToGridProgram.setUniform3f( "u_gridResolution", gridResolutionX, gridResolutionY, gridResolutionZ );
	transferToGridProgram.setUniform3f( "u_GridSize", gridWidth, gridHeight, gridDepth );
	transferToGridProgram.setUniformTexture( "u_positionTexture", particlePositionTexture, 0 );
	transferToGridProgram.setUniformTexture( "u_velocityTexture", particleVelocityTexture, 1 );

	// Not sure if right and if important
	glBlendEquation( GL_FUNC_ADD );
	glBlendFuncSeparate( GL_ONE, GL_ONE, GL_ONE, GL_ONE );
	glEnable( GL_BLEND );
	// Accumulate weight
	simulationFrameBuffer.attachTexture( weightTexture, GL_RGBA, 0 );
	simulationFrameBuffer.clearColorBuffer( ofColor( 0.0 ) );

	transferToGridProgram.setUniform1i( "u_accumulate", 0 );

	// Each particle gets splatted layer by layer from z - (SPLAT_SIZE - 1) / 2 to z + (SPLAT_SIZE - 1) / 2
	const int SPLAT_DEPTH = 5;

	for (auto z = -(SPLAT_DEPTH - 1) / 2; z <= (SPLAT_DEPTH - 1) / 2; z++) {
		transferToGridProgram.setUniform1i( "u_zOffsett", z );
		glDrawArrays( GL_POINTS, 0, particlesWidth * particlesHeight );
	}

	// Accumulate (weight * velocity)
	simulationFrameBuffer.attachTexture( tempVelocityTexture, GL_RGBA, 0 );

	transferToGridProgram.setUniform1i( "u_accumulate", 1 );



	transferToGridProgram.end();
	simulationFrameBuffer.end();

}

