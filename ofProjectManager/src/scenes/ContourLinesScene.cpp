#include "ContourLinesScene.h"

ContourLinesScene::ContourLinesScene():ofxScene("ContourLines")
{
}

void ContourLinesScene::setup()
{
	camera.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, (ofGetWidth() + ofGetHeight()) / 2);

	int spaceBetweenVetices = 10;
	for (int y = spaceBetweenVetices; y < ofGetHeight(); y += spaceBetweenVetices)
	{
		for (int x = spaceBetweenVetices; x < ofGetWidth(); x += spaceBetweenVetices)
		{
			mesh.addVertex(glm::vec3(x, y, 0));
			mesh.addColor(255);
		}
	}
}

void ContourLinesScene::update()
{
	camera.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, (ofGetWidth() + ofGetHeight()) / 2);

}

void ContourLinesScene::draw()
{

	camera.begin();
	ofDrawCircle(0, 0, 30);
	ofDrawCircle(ofGetWidth(), 0, 30);
	ofDrawCircle(ofGetWidth(), ofGetHeight(), 30);
	ofDrawCircle(0, ofGetHeight(), 30);
	mesh.drawVertices();
	camera.end();
}

void ContourLinesScene::keyPressed(int key)
{
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
