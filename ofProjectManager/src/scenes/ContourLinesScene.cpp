#include "ContourLinesScene.h"

ContourLinesScene::ContourLinesScene():ofxScene("ContourLines"),
				width(ofGetWidth()),
				height(ofGetHeight()),
				time(0.f)
{
	setSingleSetup(true);
}

void ContourLinesScene::setup()
{
	// Load Shader
	filesystem::path shader_path("../../res/shader");
	contourLineShader.load(shader_path / "contourLines.vert", shader_path / "contourLines.frag");
	
	// Set camera in the middle of the scene
	camera.disableMouseInput();
	camera.setPosition(width / 2, height / 2, (width + height) / 2);

	// Fill mesh with vertices
	spaceBetweenVetices = 5;
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			mesh.addVertex(glm::vec3(x, y, 0));
			mesh.addTexCoord({ x / width , y / height });
		}
	}
	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			mesh.addIndex(x + y * width);
			mesh.addIndex((x + 1) + y * width);
			mesh.addIndex(x + (y + 1) * width);

			mesh.addIndex((x + 1) + y * width);
			mesh.addIndex((x + 1) + (y + 1) * width);
			mesh.addIndex(x + (y + 1) * width);
		}
	}

	// Setup gui and parameters
	gui.setup();
	shaderUniforms.setName("Shader Parameters");
	shaderUniforms.add(speed.set("u_speed", 0.015, 0.00, 0.1));
	shaderUniforms.add(scale.set("u_scale", 0.005, 0.0, 0.01));
	shaderUniforms.add(amplitude.set("u_amplitude", 2.0, 0.0, 5.0));

	gui.add(shaderUniforms);
	gui.setPosition(width - gui.getWidth() - 10, height - gui.getHeight() - 10);

}

void ContourLinesScene::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	time = ofGetElapsedTimef();
}

void ContourLinesScene::draw()
{

	camera.begin(); 
	{
		contourLineShader.begin(); 
		{
			contourLineShader.setUniform1f("u_time", time);
			contourLineShader.setUniform2f("u_mouse", ofGetMouseX(), height - ofGetMouseY());
			contourLineShader.setUniforms(shaderUniforms);
			mesh.drawVertices();
		}
		contourLineShader.end();

		// Draw circles at the corners of the sceen and at mouseposition
		ofNoFill();
		ofDrawCircle(0, 0, 30);
		ofDrawCircle(width, 0, 30);
		ofDrawCircle(width, height, 30);
		ofDrawCircle(0, height, 30);
		ofDrawCircle(ofGetMouseX(), height - ofGetMouseY(), 30);

		/*
		ofSetColor(50,0, 0);
		mesh.drawVertices();
		ofSetColor(0, 50, 0);
		mesh.drawFaces();
		ofSetColor(0, 0, 50);
		mesh.drawWireframe();
		*/

	}
	camera.end();

	ofSetColor(255);
	gui.draw();
				
}

void ContourLinesScene::keyPressed(int key)
{
	// reset the camera to the middle of the scene
	if (key == 'r') {
		camera.reset();
		camera.setPosition(width / 2, height / 2, (width + height) / 2);
	}

	if (key == ofKey::OF_KEY_SHIFT){
		camera.enableMouseInput();
	}
}

void ContourLinesScene::keyReleased(int key)
{
	if (key == ofKey::OF_KEY_SHIFT){
		camera.disableMouseInput();
	}
}

void ContourLinesScene::mouseMoved(int x, int y)
{
}

void ContourLinesScene::mouseDragged(int x, int y, int button)
{
}

void ContourLinesScene::mousePressed(int x, int y, int button)
{
}

void ContourLinesScene::mouseReleased(int x, int y, int button)
{
}

void ContourLinesScene::mouseEntered(int x, int y)
{
}

void ContourLinesScene::mouseExited(int x, int y)
{
}

void ContourLinesScene::windowResized(int w, int h)
{
}

void ContourLinesScene::dragEvent(ofDragInfo dragInfo)
{
}

void ContourLinesScene::gotMessage(ofMessage msg)
{
}
