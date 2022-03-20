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

	int n_cells = grid_width * grid_height * 3;
	vector<float> cells( n_cells );
	for (size_t x = 0; x < grid_width; x++) {
		for (size_t y = 0; y < grid_height; y++) {
			size_t i = x * grid_height + y;

			cells[i * 3 + 0] = ofRandom( 1.0 );
			cells[i * 3 + 1] = ofRandom( 1.0 );
			cells[i * 3 + 2] = 0.f;
		}
	}

	velocityGrid.allocate( grid_width, grid_height, GL_RGB32F );
	velocityGrid.setTextureMinMagFilter( GL_NEAREST, GL_NEAREST );
	velocityGrid.setTextureWrap( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE );
	velocityGrid.loadData( cells.data(), grid_width, grid_height, GL_RGB );
}

void FluidScene::calculateGridDimensions() {

	float aspect = (width * 1.f) / (height * 1.f);

	//grid_height = ceil( sqrt( width * height ) / (cells * 1.f * aspect) ); // rounding up

	//while (cells > floor( width / (aspect * grid_height) ))
	//	grid_height--;

	grid_width = sqrt( (cells * 1.f) * aspect );

	grid_height = (grid_width * 1.f) / aspect;
	cells = grid_width * grid_height;

}

void FluidScene::update()
{
	glm::vec2 center = glm::vec2( width, height ) * 0.5f;

	for (Particle& p : particles) {
		if (p.pos.x > width || p.pos.x < 0
			|| p.pos.y > height || p.pos.y < 0) p.pos = center;

	}
}

void FluidScene::draw()
{
	ofBackground( 0 );

	camera.begin();
	ofNoFill();
	ofDrawRectangle( 0, 0, width, height );

	for (Particle const& p : particles) {
		ofDrawCircle( p.pos, 3 );
	}

	velocityGrid.draw( 0, 0, width, height );

	camera.end();
}
