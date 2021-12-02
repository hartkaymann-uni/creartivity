#include "WorleyScene.h"

WorleyScene::WorleyScene()
	: ofxFadeScene( "Worley" ),
	width( ofGetWidth() ),
	height( ofGetHeight() )
{
	setSingleSetup( false );
	setFade(1000, 1000);
}

void WorleyScene::setup()
{
	// Load Shaders
	filesystem::path shader_path( "../../res/shader" );
	worleyShader.load( shader_path / "worley.vert", shader_path / "worley.frag" );

	
	camera.disableMouseInput();
	camera.setupPerspective();
	camera.setPosition(0, 0, 665);
	camera.setFarClip(ofGetWidth() * 10);

	for (int y = 0; y < (height); y += 100) {
		for (int x = 0; x < (width); x += 100) {
			if (y % 200 == 0){
				Node newHub(x, y);
				nodes.push_back(newHub);
			}else{
				Node newHub(x+50, y);
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
		nodes[i].update( i );

		positions[i] = nodes[i].position;
	}

	width = ofGetWidth();
	height = ofGetHeight();

}

void WorleyScene::draw()
{

	ofBackground( 0 );

	camera.begin();
	worleyShader.begin();
	worleyShader.setUniform2f( "u_resolution", (float)ofGetWidth(), (float)ofGetHeight());
	worleyShader.setUniform1f( "u_time", ofGetElapsedTimef() );
	worleyShader.setUniform2f( "u_mouse", ofGetMouseX(), ofGetMouseY() );

	worleyShader.setUniform1i( "u_length", nodes.size() );
	worleyShader.setUniform2fv( "u_positions",  &positions[0].x, 88);

	ofFill();
	ofDrawRectangle( 0, 0, width, height );

	worleyShader.end();
	
	
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].draw();
	}
	camera.end();
	float ratio = width / height;
	//  300 :  400 -> 0,75 --->  4x3	100
	// 1000 : 1000 -> 1,00 ---> 10x10
	// 1

	/*
	int boxSize =  100;
	for (int x = 0; x < (width); x += boxSize) {
		ofDrawLine(x + boxSize / 2, 0 , x + boxSize / 2, height);
		
	}
	for (int y = 0; y < (height); y += boxSize) {
		ofDrawLine(0, y + boxSize / 2, width, y + boxSize / 2);
	}
	*/

}

void WorleyScene::keyPressed( int key )
{
	// std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
	}
}

void WorleyScene::keyReleased( int key )
{
}

void WorleyScene::mouseMoved( int x, int y )
{
}

void WorleyScene::mouseDragged( int x, int y, int button )
{
}

void WorleyScene::mousePressed( int x, int y, int button )
{
}

void WorleyScene::mouseReleased( int x, int y, int button )
{
}

void WorleyScene::mouseEntered( int x, int y )
{
}

void WorleyScene::mouseExited( int x, int y )
{
}

void WorleyScene::windowResized( int w, int h )
{
	width = w;
	height = h;
}

void WorleyScene::dragEvent( ofDragInfo dragInfo )
{
}

void WorleyScene::gotMessage( ofMessage msg )
{
}

Node::Node( int startX, int startY ) {
	float directionSpeed = 1.0;
	boxMid = glm::vec2( startX, startY );
	position = glm::vec2( startX, startY );
	direction = glm::vec2(0,0);
}

Node::~Node() {
}

void Node::update( int i ) {

	int boxSize = 100;
	direction.x = (boxSize / 5.0 ) * sin( 0.5 * ofGetElapsedTimef());
	direction.y = (boxSize / 5.0 ) * cos( 0.5 * ofGetElapsedTimef());
	position.x = boxMid.x + direction.x;
	position.y = boxMid.y + direction.y;
}

void Node::draw() {
	ofNoFill();
	ofDrawCircle( position, 10 );
}