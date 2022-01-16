#include "CuboidScene.h"

CuboidScene::CuboidScene() : ccScene( "Cuboids" )
{
	filesystem::path shader_path = getShaderPath();
	cuboidShader.load( shader_path / "cuboids.vert", shader_path / "cuboids.frag" );

	uniforms.add( gridDimensions.set( "grid", { 100.0, 100.0 } ) );
	uniforms.add( cubeDimensions.set( "cube", { width / gridDimensions.get().x, 10.0, height / gridDimensions.get().y } ) );

}

void CuboidScene::setup() {

	renderFbo.allocate( width, height, GL_RGBA );

	gui.setup();

	ofBoxPrimitive box( cubeDimensions.get().x, cubeDimensions.get().y, cubeDimensions.get().z );
	cube = box.getMesh();

}

void CuboidScene::update() {

	renderFbo.begin();
	ofClear( 0 );

	camera.begin();
	cuboidShader.begin();
	cuboidShader.setUniforms( uniforms );

	cube.drawInstanced( OF_MESH_FILL, gridDimensions.get().x * gridDimensions.get().y );

	cuboidShader.end();

	camera.end();
	renderFbo.end();
}

void CuboidScene::draw()
{
	renderFbo.draw( 0, 0 );
}


void CuboidScene::keyPressed( int key ) {

	// std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
	}
	else if (key == 'r' || key == 'R') {
		std::cout << "Before: " << camera.getPosition() << std::endl;
		resetCamera();
		std::cout << "After: " << camera.getPosition() << std::endl;
	}
}

void CuboidScene::keyReleased( int key ) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();
	}
}
