#include "ContourLinesScene.h"

ContourLinesScene::ContourLinesScene(int w, int h) : ccScene( "ContourLines" ),
time( 0.f ),
grid({ w, h })
{}

void ContourLinesScene::setup()
{
	// Load Shader
	filesystem::path shader_path = getShaderPath();
	bool loaded = contourLineShader.load( shader_path / "contour.vert", shader_path / "contour.frag" );	

	plane = ofPlanePrimitive(width, height, grid.x, grid.y);
	plane.setPosition(width / 2, height / 2, 0.f);

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

#if 0
	map<int, ccUser>* users = userManager->getUsers();
	std::map<int, ccUser>::iterator it = users->begin();
	std::map<int, ccUser>::iterator itEnd = users->end();
	while (it != itEnd) {

		ofVec2f left = it->second.left();
		ofVec2f right = it->second.right();

		left.x *= width;
		right.x *= width;
		left.y = height - (left.y * height);
		right.y = height - (right.y * height);

		for (size_t i = 0; i < mesh.getNumVertices(); i++) {
			ofVec3f v = mesh.getVertex( i );

			float distance = min( v.distance( left ), v.distance( right ) );
			if (distance < p_MouseRadius.get()) {
				v.z -= ofMap( distance, p_MouseRadius.get(), 0.0, 0.0, 1.0);
			}

			mesh.setVertex( i, v );
		}
		it++;
	}
#endif
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