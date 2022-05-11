#include "ContourLinesScene.h"

ContourLinesScene::ContourLinesScene(int w, int h) : ccScene( "ContourLines" ),
time( 0.f ),
grid({ w, h })
{}

void ContourLinesScene::setup()
{
	// Load Shader
	filesystem::path shader_path = getShaderPath();
	bool err_cont	= contourLineShader.load( shader_path / "contour.vert", shader_path / "contour.frag" );	
	bool err_splat	= splatShader.load( shader_path / "passthru.vert", shader_path / "splat.frag" );	
	bool err_sub	= subtractShader.load( shader_path / "passthru.vert", shader_path / "subtract.frag" );	

	// Create mesh
	plane = ofPlanePrimitive(width, height, grid.x, grid.y);
	plane.setPosition(width / 2, height / 2, 0.f);

	ofDisableArbTex();
	interaction.allocate( grid, GL_R32F);
	ofEnableArbTex();

	// Setup gui and parameters
	shaderUniforms.setName( "Shader Parameters" );
	shaderUniforms.add( p_Speed.set( "u_speed", 0.015, 0.00, 0.1 ) );
	shaderUniforms.add( p_Scale.set( "u_scale", 0.01, 0.0, 0.05 ) );
	shaderUniforms.add( p_Amplitude.set( "u_amplitude", 50.0, 0.0, 100.0 ) );
	shaderUniforms.add( p_MouseRadius.set( "u_radius", 0.01, 0.0, 1.0 ) );
	shaderUniforms.add( p_MouseStrength.set( "u_strength", 0.1, 0.0, 1.0 ) );

	gui.add( shaderUniforms );
	gui.setPosition( width - gui.getWidth() - 10, height - gui.getHeight() - 10 );
}

void ContourLinesScene::update()
{
	time = ofGetElapsedTimef();

	// Apply interaction for all users
	vector<ccUser> u = userManager->getUserVec();
	for (vector<ccUser>::iterator it = u.begin(); it != u.end(); it++) {

		glm::vec2 left( it->getPositons().first );
		glm::vec2 right( it->getPositons().second );

		splatShader.begin();

		splatShader.setUniformTexture( "read", interaction.read->getTexture(), 2 );
		splatShader.setUniform2f("grid", grid);
		splatShader.setUniform2f("left", left);
		splatShader.setUniform2f("right", right);
		splatShader.setUniform1f("radius", p_MouseRadius);
		splatShader.setUniform1f("strength", p_MouseStrength);

		interaction.write->begin();
		ofClear(0);
		ofFill();
		plane.draw();
		interaction.write->end();
		interaction.swap();

		splatShader.end();
	}


	// Flatten terrain
	subtractShader.begin();
	subtractShader.setUniformTexture("read", interaction.read->getTexture(), 2);
	subtractShader.setUniform2f("grid", grid);

	interaction.write->begin();
	plane.draw();
	interaction.write->end();
	interaction.swap();

	subtractShader.end();
}

void ContourLinesScene::draw()
{
	camera.begin();
	{
		contourLineShader.begin();
		{
			contourLineShader.setUniform1f( "u_time", time );
			contourLineShader.setUniform2f( "u_mouse", ofGetMouseX(), height - ofGetMouseY() );
			contourLineShader.setUniforms( shaderUniforms );

			ofDisableAlphaBlending();
			
			if(!wireframeShading) plane.draw();
			else plane.drawWireframe();
		}
		contourLineShader.end();
	}
	camera.end();

	interaction.read->draw(0, 0, width, height);
}

void ContourLinesScene::keyPressed( int key )
{
	// reset the camera to the middle of the scene
	switch(key) {
	case 'r':
		camera.reset();
		camera.setPosition( width / 2, height / 2, (width + height) / 2 );
		break;
	case ofKey::OF_KEY_SHIFT:
		camera.enableMouseInput();
		break;
	case 's':
		changeShading();
		break;
	}
}

void ContourLinesScene::keyReleased( int key )
{
	if (key == ofKey::OF_KEY_SHIFT) {
		camera.disableMouseInput();
	}
}

void ContourLinesScene::changeShading() {
	wireframeShading = !wireframeShading;
}