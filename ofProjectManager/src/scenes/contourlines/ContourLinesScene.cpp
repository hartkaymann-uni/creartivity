#include "ContourLinesScene.h"

ContourLinesScene::ContourLinesScene(int w, int h) : ccScene( "ContourLines" ),
time( 0.f ),
grid({ w, h })
{}

void ContourLinesScene::setup()
{
	// Load Shader
	filesystem::path shader_path = getShaderPath();
	bool err_contour = contourLineShader.load( shader_path / "contour.vert", shader_path / "contour.frag" );	
	bool err_splat = contourLineShader.load( shader_path / "passthru.vert", shader_path / "splat.frag" );	

	// Create mesh
	plane = ofPlanePrimitive(width, height, grid.x, grid.y);
	plane.setPosition(width / 2, height / 2, 0.f);

	interaction.allocate( glm::vec2( width, height ) );

	// Setup gui and parameters
	shaderUniforms.setName( "Shader Parameters" );
	shaderUniforms.add( p_Speed.set( "u_speed", 0.015, 0.00, 0.1 ) );
	shaderUniforms.add( p_Scale.set( "u_scale", 0.01, 0.0, 0.05 ) );
	shaderUniforms.add( p_Amplitude.set( "u_amplitude", 50.0, 0.0, 100.0 ) );
	shaderUniforms.add( p_MouseRadius.set( "u_radius", 100.0, 25.0, 500.0 ) );
	shaderUniforms.add( p_Thickness.set( "u_thickness", 1.0, 0.1, 5.0 ) );

	gui.add( shaderUniforms );
	gui.setPosition( width - gui.getWidth() - 10, height - gui.getHeight() - 10 );
}

void ContourLinesScene::update()
{
	time = ofGetElapsedTimef();

	// Apply interaction for all users
	vector<ccUser> u = userManager->getUserVec();
	for (vector<ccUser>::iterator it = u.begin(); it != u.end(); it++) {
		interaction.write->begin();

		glm::vec2 left( it->getPositons().first );
		glm::vec2 right( it->getPositons().second );

		splat.begin();

		splat.setUniformTexture( "read", interaction.read->getTexture(), 0 );

		splat.end();
		interaction.write->end();
		interaction.swap();
	}
}

void ContourLinesScene::draw()
{

	camera.begin();
	{
		contourLineShader.begin();
		{
			contourLineShader.setUniform1f( "u_time", time );
			contourLineShader.setUniform2f( "u_mouse", ofGetMouseX(), height - ofGetMouseY() );
			//contourLineShader.setUniform2fv("hands", &user_positions[0].x, sizeof(ofVec2f) * 10);
			contourLineShader.setUniforms( shaderUniforms );

			ofDisableAlphaBlending();
			glEnable(GL_POINT_SMOOTH);
			
			if(!wireframeShading) plane.draw();
			else plane.drawWireframe();
		}
		contourLineShader.end();
	}
	camera.end();
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