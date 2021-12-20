#include "ContourLinesScene.h"

ContourLinesScene::ContourLinesScene() : ccScene( "ContourLines" ),
time( 0.f ),
sbv( 0 ),
meshWidth( 0 ),
meshHeight( 0 ),
count( 0 )
{}

void ContourLinesScene::setup()
{
	ofSetFrameRate( 60 );

	// Load Shader
	filesystem::path shader_path( "../../res/shader" );
	bool shadersloaded = contourLineShader.load( shader_path / "contour.vert", shader_path / "contour.frag" );

	// Set camera in the middle of the scene
	camera.disableMouseInput();
	camera.setPosition( width / 2, height / 2, (width + height) / 2 );

	// Set variables
	count = 1;
	sbv = 3; // SpaceBetweenVetices 
	meshWidth = width / sbv + 1;
	meshHeight = height / sbv + 1;

	// Fill mesh with vertices
	mesh.setMode( OF_PRIMITIVE_TRIANGLES );
	for (int y = 0; y < meshHeight; y++) {
		for (int x = 0; x < meshWidth; x++) {
			mesh.addVertex( glm::vec3( x * sbv, y * sbv, 0 ) );
			mesh.addTexCoord( { (x * sbv) / width , (y * sbv) / height } );
		}
	}
	for (int y = 0; y < meshHeight - 1; y++) {
		for (int x = 0; x < meshWidth - 1; x++) {
			mesh.addIndex( x + y * meshWidth );
			mesh.addIndex( (x + 1) + y * meshWidth );
			mesh.addIndex( x + (y + 1) * meshWidth );

			mesh.addIndex( (x + 1) + y * meshWidth );
			mesh.addIndex( (x + 1) + (y + 1) * meshWidth );
			mesh.addIndex( x + (y + 1) * meshWidth );
		}
	}

	// Setup gui and parameters
	gui.setup();
	shaderUniforms.setName( "Shader Parameters" );
	shaderUniforms.add( speed.set( "u_speed", 0.015, 0.00, 0.1 ) );
	shaderUniforms.add( scale.set( "u_scale", 0.01, 0.0, 0.05 ) );
	shaderUniforms.add( amplitude.set( "u_amplitude", 10.0, 0.0, 20.0 ) );
	shaderUniforms.add( radius.set( "u_radius", 100.0, 25.0, 500.0 ) );
	shaderUniforms.add( thickness.set( "u_thickness", 1.0, 0.1, 5.0 ) );
	shaderUniforms.add( limit.set( "u_limit", 3.5, 0.0, 10.0 ) );

	gui.add( shaderUniforms );
	gui.setPosition( width - gui.getWidth() - 10, height - gui.getHeight() - 10 );
}

void ContourLinesScene::update()
{
	time = ofGetElapsedTimef();
	updateUserPositions();

	// TODO: somehow combine mouse and user interaction so this function isn't as long and ugly anymore
	std::unique_ptr<std::map<int, user>> users = receiver->getUsers();
	std::map<int, user>::iterator it = users->begin();
	std::map<int, user>::iterator itEnd = users->end();
	while (it != itEnd) {

		ofVec2f left = it->second.positionLeft;
		ofVec2f right = it->second.positionRight;

		left.x *= width;
		right.x *= width;
		left.y = height - (left.y * height);
		right.y = height - (right.y * height);

		for (size_t i = 0; i < mesh.getNumVertices(); i++) {
			ofVec3f v = mesh.getVertex( i );

			float distance = min( v.distance( left ), v.distance( right ) );
			if (distance < radius) {
				v.z -= ofMap( distance, radius, 0.0, 0.0, 1.0 );
			}

			mesh.setVertex( i, v );
		}
		it++;
	}

	ofVec3f mouse = ofVec3f( ofGetMouseX(), height - ofGetMouseY(), 0.0f );

	for (size_t i = 0; i < mesh.getNumVertices(); i++) {
		ofVec3f v = mesh.getVertex( i );
		mouse.z = v.z;

		float distance = v.distance( mouse );
		if (distance < radius) {
			//if (v.z >= ofMap(distance, 0.0, radius, -1.0 * limit, 0.0))
			v.z -= ofMap( distance, radius, 0.0, 0.0, 1.0 );
		}

		if (v.z < 0.f) {
			v.z += 0.05 * limit;
		}

		mesh.setVertex( i, v );
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

			mesh.draw();
		}
		contourLineShader.end();

		// Draw circles at the corners of the sceen and at mouseposition
		ofNoFill();
		ofDrawCircle( 0, 0, 30 );
		ofDrawCircle( width, 0, 30 );
		ofDrawCircle( width, height, 30 );
		ofDrawCircle( 0, height, 30 );
		ofDrawCircle( ofGetMouseX(), height - ofGetMouseY(), 30 );

		/*
		ofSetColor(255,0, 0);
		//mesh.drawVertices();
		ofSetColor(0, 50, 0);
		//mesh.drawFaces();
		ofSetColor( 255 );
		mesh.drawWireframe();
		*/
	}
	camera.end();

	ofSetColor( 255 );
}

void ContourLinesScene::keyPressed( int key )
{
	// reset the camera to the middle of the scene
	if (key == 'r') {
		camera.reset();
		camera.setPosition( width / 2, height / 2, (width + height) / 2 );
	}

	if (key == ofKey::OF_KEY_SHIFT) {
		camera.enableMouseInput();
	}
}

void ContourLinesScene::keyReleased( int key )
{
	if (key == ofKey::OF_KEY_SHIFT) {
		camera.disableMouseInput();
	}
}

void ContourLinesScene::mouseMoved( int x, int y )
{
}

void ContourLinesScene::mouseDragged( int x, int y, int button )
{
}

void ContourLinesScene::mousePressed( int x, int y, int button )
{
}

void ContourLinesScene::mouseReleased( int x, int y, int button )
{
}

void ContourLinesScene::mouseEntered( int x, int y )
{
}

void ContourLinesScene::mouseExited( int x, int y )
{
}

void ContourLinesScene::windowResized( int w, int h )
{
}

void ContourLinesScene::dragEvent( ofDragInfo dragInfo )
{
}

void ContourLinesScene::gotMessage( ofMessage msg )
{
}