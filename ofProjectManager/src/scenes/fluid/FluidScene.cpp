#include "FluidScene.h"

FluidScene::FluidScene() :
	ccScene( "Fluid" ),
	cells( NUM_CELLS ),
	grid_width( 0 ),
	grid_height( 0 ) {}

void FluidScene::setup()
{
	for (int i = 0; i < NUM_PARTICLES; i++) {
		Particle p = {
			{ofRandom( width ), ofRandom( height )},
		};

		particles.push_back( p );
	}

	calculateGridDimensions();

	velocityGrid.allocate( grid_width, grid_height, GL_RGB32F );
	velocityGrid.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	velocityGrid.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	pressureGrid.allocate( grid_width, grid_height, GL_RGB32F );
	pressureGrid.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	pressureGrid.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	vorticityGrid.allocate( grid_width, grid_height, GL_RGB32F );
	vorticityGrid.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	vorticityGrid.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	forceGrid.allocate( grid_width, grid_height, GL_RGB32F );
	forceGrid.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	forceGrid.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );

	// Create shader programs
	filesystem::path shaderPath = getShaderPath();
	string module = "Fluid Shader Loading";

	bool err_adv = advectProgram.load( shaderPath / "passthru.vert", shaderPath / "advect.frag" );
	if (!err_adv) {
		(void)ofLogError( module, "Failed to load advection shader!" );
	}

	bool err_jac = jacobiProgram.load( shaderPath / "passthru.vert", shaderPath / "jacobi.frag" );
	if (!err_jac) {
		(void)ofLogError( module, "Failed to load jacobi shader!" );
	}

	bool err_for = forceProgram.load( shaderPath / "passthru.vert", shaderPath / "force.frag" );
	if (!err_for) {
		(void)ofLogError( module, "Failed to load force shader!" );
	}

	bool err_div = divergenceProgram.load( shaderPath / "passthru.vert", shaderPath / "divergence.frag" );
	if (!err_div) {
		(void)ofLogError( module, "Failed to load divergence shader!" );
	}

	bool err_grad = gradientProgram.load( shaderPath / "passthru.vert", shaderPath / "gradient.frag" );
	if (!err_grad) {
		(void)ofLogError( module, "Failed to load gradient shader!" );
	}
	bool err_bounds = boundariesProgram.load( shaderPath / "passthru.vert", shaderPath / "boundaries.frag" );
	if (!err_bounds) {
		(void)ofLogError( module, "Failed to load boundaries shader!" );
	}
}

void FluidScene::calculateGridDimensions() {

	float aspect = (width * 1.f) / (height * 1.f);

	grid_width = sqrt( (cells * 1.f) * aspect );

	grid_height = (grid_width * 1.f) / aspect;
	cells = grid_width * grid_height;

}

void FluidScene::update()
{


}

void FluidScene::draw()
{
	ofBackground( 0 );

	camera.begin();
	ofNoFill();
	ofDrawRectangle( 0, 0, width, height );

	velocityGrid.draw( 0, 0, width * 0.5f, height * 0.5f );
	pressureGrid.draw( width * 0.5f, 0, width * 0.5f, height * 0.5f );
	vorticityGrid.draw( 0, height * 0.5f, width * 0.5f, height * 0.5f );
	forceGrid.draw( width * 0.5f, height * 0.5f, width * 0.5f, height * 0.5f );

	camera.end();
}
