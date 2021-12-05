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
	filesystem::path shader_path("../../res/shader");
	contourLineShader.load(shader_path / "contourLines.vert", shader_path / "contourLines.frag");
	
	// set camera in the middle of the scene
	camera.setPosition(width / 2, height / 2, (width + height) / 2);

	// fill mesh with vertices
	spaceBetweenVetices = 5;
	for (int y = spaceBetweenVetices; y < height; y += spaceBetweenVetices)
	{
		for (int x = spaceBetweenVetices; x < width; x += spaceBetweenVetices)
		{
			mesh.addVertex(glm::vec3(x, y, 0));
			mesh.addTexCoord({ x / width , y / height });
		}
	}

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
			contourLineShader.setUniform1f("time", time);
			contourLineShader.setUniforms(shaderUniforms);
			mesh.drawVertices();
		}
		contourLineShader.end();

		// draw circles at the corners of the sceen
		ofNoFill();
		ofDrawCircle(0, 0, 30);
		ofDrawCircle(width, 0, 30);
		ofDrawCircle(width, height, 30);
		ofDrawCircle(0, height, 30);

		ofDrawCircle(ofGetMouseX(), height - ofGetMouseY(), 30);
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
}

void ContourLinesScene::keyReleased(int key)
{

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
