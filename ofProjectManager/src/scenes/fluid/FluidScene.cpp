#include "FluidScene.h"

FluidScene::FluidScene() :
	ccScene( "Fluid" ),
	time( 0.f ),
	dt( 0.f ),
	debug( true ),
	step( false ) {}

void FluidScene::setup()
{
	grid = { {512, 256}, 1.f, false };

	// Create render targets
	{
		int no_pixels = grid.size.x * grid.size.y * 3;
		vector<float> cells( no_pixels );
		for (size_t x = 0; x < grid.size.y; x++) {
			for (size_t y = 0; y < grid.size.x; y++) {
				size_t i = x * grid.size.x + y;
				float initialValue = ofMap( x, 0, grid.size.y, 0.0, 1.0, true );
				cells[i * 3 + 0] = initialValue;
				cells[i * 3 + 1] = initialValue;
				cells[i * 3 + 2] = initialValue;
			}
		}

		velocity.allocate( grid.size.x, grid.size.y, GL_RGB32F );
		velocity.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
		velocity.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
		velocity.getTexture().loadData( cells.data(), grid.size.x, grid.size.y, GL_RGB );
	}

	{
		int no_pixels = grid.size.x * grid.size.y * 3;
		vector<float> cells( no_pixels );
		for (size_t x = 0; x < grid.size.y; x++) {
			for (size_t y = 0; y < grid.size.x; y++) {
				size_t i = x * grid.size.x + y;
				float initialValue = ofNoise( 2.f * x / grid.size.y, 2.f * y / grid.size.x );

				cells[i * 3 + 0] = initialValue;
				cells[i * 3 + 1] = initialValue;
				cells[i * 3 + 2] = initialValue;
			}
		}

		density.allocate( grid.size.x, grid.size.y, GL_RGB32F );
		density.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
		density.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
		density.getTexture().loadData( cells.data(), grid.size.x, grid.size.y, GL_RGB );

	}
	velocityDivergence.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	velocityDivergence.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	velocityDivergence.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	velocityVorticity.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	velocityVorticity.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	velocityVorticity.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	pressure.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	pressure.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	pressure.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	// Create shader programs
	filesystem::path shaderPath = getShaderPath();

	bool err_adv = advectProgram.load( shaderPath / "passthru.vert", shaderPath / "advect.frag" );
	bool err_grad = gradientProgram.load( shaderPath / "passthru.vert", shaderPath / "gradient.frag" );
	bool err_jacs = jacobiscalarProgram.load( shaderPath / "passthru.vert", shaderPath / "jacobiscalar.frag" );
	bool err_jacv = jacobivectorProgram.load( shaderPath / "passthru.vert", shaderPath / "jacobivector.frag" );
	bool err_div = divergenceProgram.load( shaderPath / "passthru.vert", shaderPath / "divergence.frag" );
	bool err_vor = vorticityProgram.load( shaderPath / "passthru.vert", shaderPath / "vorticity.frag" );
	bool err_vorf = vorticityforceProgram.load( shaderPath / "passthru.vert", shaderPath / "vorticityforce.frag" );
	bool err_bounds = boundariesProgram.load( shaderPath / "passthru.vert", shaderPath / "boundaries.frag" );
}

void FluidScene::update()
{
	// Timer
	dt = (clock() - time) / CLOCKS_PER_SEC;
	time = clock();

	if (debug && !step)
		return;

	// Dissipation only affects density carried by the velocity field
	float temp = dissipation;
	dissipation = 1;
	advect( velocity, velocity );
	boundary( velocity, velocity, -1.f );

	dissipation = temp;
	advect( density, density );

	// Add external forces
	// TODO: Implement mouse interaction here

	if (applyVorticity) {
		vortex( velocityVorticity );
		vortexConfine( velocityVorticity, velocity );
		boundary( velocity, velocity, -1.f );
	}

	if (applyViscosity && viscosity > 0.f) {
		float s = grid.scale;

		float alpha = (s * s) / (viscosity * dt);
		float beta = 4.f + alpha;

		diffuse( velocity, velocity, velocity, alpha, beta, -1.f );
	}

	project();

	if (debug) step = false;
}

void FluidScene::advect( ofFbo& advected, ofFbo& output ) {
	advectProgram.begin();

	advectProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	advectProgram.setUniformTexture( "advected", advected.getTexture(), 1 );
	advectProgram.setUniform2f( "gridSize", grid.size );
	advectProgram.setUniform1f( "gridScale", grid.scale );
	advectProgram.setUniform1f( "timestep", dt );
	advectProgram.setUniform1f( "dissipation", dissipation );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	ofDrawRectangle( 0, 0, grid.size.x, grid.size.y );
	outBuffer.end();

	output = outBuffer;

	advectProgram.end();
}

void FluidScene::boundary( ofFbo& input, ofFbo& output, float scale ) {
	if (!grid.applyBounds)
		return;

	boundariesProgram.begin();

	boundariesProgram.setUniformTexture( "read", input.getTexture(), 0 );
	boundariesProgram.setUniform2f( "gridSize", grid.size );
	boundariesProgram.setUniform1f( "scale", scale );
	// do this for all offsets, see boundary slabop l.78

	output.begin();

	ofDrawRectangle( 0, 0, grid.size.x, grid.size.y );

	output.end();

	boundariesProgram.end();
}

void FluidScene::vortex( ofFbo& output ) {
	vorticityProgram.begin();

	vorticityProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	vorticityProgram.setUniform2f( "gridSize", grid.size );
	vorticityProgram.setUniform1f( "gridScale", grid.scale );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	ofDrawRectangle( 0, 0, grid.size.x, grid.size.y );
	outBuffer.end();

	output = outBuffer;

	vorticityProgram.end();
}

void FluidScene::vortexConfine( ofFbo& vorticity, ofFbo& output ) {
	vorticityforceProgram.begin();

	vorticityforceProgram.setUniformTexture( "velocity", velocity.getTexture(), 2 );
	vorticityforceProgram.setUniformTexture( "vorticity", vorticity.getTexture(), 1 );
	vorticityforceProgram.setUniform2f( "gridSize", grid.size );
	vorticityforceProgram.setUniform1f( "gridScale", grid.scale );
	vorticityforceProgram.setUniform1f( "timestep", dt );
	vorticityforceProgram.setUniform1f( "epsilon", epsilon );
	vorticityforceProgram.setUniform2f( "curl", curl * grid.scale, curl * grid.scale );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	ofDrawRectangle( 0, 0, grid.size.x, grid.size.y );
	outBuffer.end();

	output = outBuffer;


	vorticityforceProgram.end();
}

void FluidScene::diffuse( ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta, float scale ) {
	for (int i = 0; i < jacobiIterations; i++) {
		diffuseStep( x, b, output, alpha, beta );
	}
}

void FluidScene::diffuseStep( ofFbo& x, ofFbo& b, ofFbo& output, float alpha, float beta ) {
	jacobivectorProgram.begin();

	jacobivectorProgram.setUniformTexture( "x", x.getTexture(), 2 );
	jacobivectorProgram.setUniformTexture( "b", b.getTexture(), 1 );
	jacobivectorProgram.setUniform2f( "gridSize", grid.size );
	jacobivectorProgram.setUniform1f( "gridScale", grid.scale );
	jacobivectorProgram.setUniform1f( "alpha", alpha );
	jacobivectorProgram.setUniform1f( "beta", beta );

	ofFbo outBuffer;
	outBuffer.allocate( grid.size.x, grid.size.y, GL_RGB32F );
	outBuffer.getTexture().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	outBuffer.getTexture().setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	outBuffer.begin();
	ofClear( 0 );
	ofFill();
	ofDrawRectangle( 0, 0, grid.size.x, grid.size.y );
	outBuffer.end();

	output = outBuffer;

	jacobivectorProgram.end();

}

void FluidScene::project() {

}

void FluidScene::draw()
{
	ofBackground( 0 );

	camera.begin();
	ofNoFill();
	ofDrawRectangle( 0, 0, width, height );

	density.draw( 0.f, 0.f, grid.size.x, grid.size.y );
	velocity.draw( 0.f, grid.size.y, grid.size.x, grid.size.y );
	velocityVorticity.draw( grid.size.x, 0.f, grid.size.x, grid.size.y );

	camera.end();
}

void FluidScene::keyPressed( int key ) {

	//std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
	}
	else if (key == 'r' || key == 'R') {
		resetCamera();
	}
	else if (key == 'd' || key == 'D') {
		debug = !debug;
	}
	else if (key == 32) {
		if (debug) step = true;
	}
}

void FluidScene::keyReleased( int key ) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();
	}
}
