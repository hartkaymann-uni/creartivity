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

}

void WorleyScene::update()
{
	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );
}

void WorleyScene::draw()
{
	ofBackground( 0 );

	worleyShader.begin();
	worleyShader.setUniform2f( "screen", (float)width, (float)height );

	ofDrawRectangle( 0, 0, width, height );

	worleyShader.end();
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
