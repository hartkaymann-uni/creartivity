#include "WorleyScene.h"

WorleyScene::WorleyScene() 
	: ofxFadeScene("Worley"),
	width(ofGetWidth()),
	height(ofGetHeight()) 
{
	setSingleSetup( false );
	setFade( 1000, 1000 );
}

void WorleyScene::setup()
{
	// Load Shaders
	filesystem::path shader_path( "../../res/shader" );
	worleyShader.load( shader_path / "worley.vert", shader_path / "worley.frag" );


	for (int y = 0; y < (height); y += 100) {
		for (int x = 0; x < (width); x += 100) {
			if (y % 200 == 0){
				Node newHub(x+50, y);
				nodes.push_back(newHub);
			}
			else{
				Node newHub(x, y);
				nodes.push_back(newHub);
			}
		}
	}

}

void WorleyScene::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].update(i);
	}

}

void WorleyScene::draw()
{
	
	ofBackground( 0 );

	worleyShader.begin();
	worleyShader.setUniform2f( "u_resolution", (float)width, (float)height );
	worleyShader.setUniform1f( "u_time", ofGetElapsedTimef());
	worleyShader.setUniform2f( "u_mouse", ofGetMouseX(), ofGetMouseY());

	worleyShader.end();

	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].draw();
	}

	int boxSize = 100;
	for (int x = 0; x < (width); x += boxSize) {
		ofDrawLine(x + boxSize / 2, 0 , x + boxSize / 2, height);
	}
	for (int y = 0; y < (height); y += boxSize) {
		ofDrawLine(0, y + boxSize / 2, width, y + boxSize / 2);
	}
}

void WorleyScene::keyPressed( int key ) 
{	
}

void WorleyScene::keyReleased( int key ) 
{
}

void WorleyScene::mouseMoved(int x, int y)
{
}

void WorleyScene::mouseDragged(int x, int y, int button)
{
}

void WorleyScene::mousePressed(int x, int y, int button)
{
}

void WorleyScene::mouseReleased(int x, int y, int button)
{
}

void WorleyScene::mouseEntered(int x, int y)
{
}

void WorleyScene::mouseExited(int x, int y)
{
}

void WorleyScene::windowResized(int w, int h)
{
}

void WorleyScene::dragEvent(ofDragInfo dragInfo)
{
}

void WorleyScene::gotMessage(ofMessage msg)
{
}

Node::Node(int startX, int startY) {
	float directionSpeed = 1.0;
	boxMid = glm::vec2(startX, startY);
	position = glm::vec2(startX, startY);
	direction = glm::vec2(ofRandom(-directionSpeed, directionSpeed), ofRandom(-directionSpeed, directionSpeed));
}

Node::~Node() {
}

void Node::update(int i) {
	
	int boxSize = 100;
	direction.x = (boxSize / 5 - 10) * (sin(0.5 * ofGetElapsedTimef() + (boxMid.y + 265) / 10 + i) + sin((i + 13) / boxMid.y * ofGetElapsedTimef() - i)) / 2;
	direction.y = (boxSize / 5 - 10) * (cos(0.5 * ofGetElapsedTimef() + (boxMid.x - 135) / 10 + i) + cos((i + 19) / boxMid.x * ofGetElapsedTimef() - i)) / 2;
	position.x = boxMid.x + direction.x;
	position.y = boxMid.y + direction.y;
}

void Node::draw() {
	ofNoFill();
	ofDrawCircle(position, 10);
}