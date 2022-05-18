#include "ccScene.h"

ccScene::ccScene( std::string name )
	: ofxScene( name ),
	width( ofGetWidth() ),
	height( ofGetHeight() ),
	mouseIsDown( false ),
	mousePosition( 0.f, 0.f, 0.f ),
	scenesPath( "../../src/scenes" )
{
	setSingleSetup( true );
	camera.disableMouseInput();
	camera.enableOrtho();
	camera.setPosition( width / 2, height / 2, (width + height) / 2 );
	camera.setNearClip( -10 * ofGetWidth() );
	camera.setFarClip( ofGetWidth() * 10 );

	gui.setup();
}

void ccScene::resetCamera()
{
	camera.reset();
	camera.setPosition( width / 2, height / 2, (width + height) / 2 );
}

ofVec3f ccScene::getProjectedPosition( ofVec3f p ) {
	glm::vec3 pos = camera.screenToWorld( p );
	pos.z = 0.0;
	return ofVec3f( pos );
}

ofVec3f ccScene::getNormalToWorldPosition(ofVec2f normalPos) {
	ofVec2f cleanNormalPos = ofVec2f(min(max(normalPos.x,0.f), 1.f), min(max(normalPos.y, 0.f), 1.f));

	ofVec2f screenPos = ofVec2f(cleanNormalPos.x * ofGetWidth(), cleanNormalPos.y * ofGetHeight());

	return getProjectedPosition(screenPos);
}

void ccScene::updateUserPositions()
{
	map<int, ccUser>* users = userManager->getUsers();
	std::map<int, ccUser>::iterator it = users->begin();
	std::map<int, ccUser>::iterator itEnd = users->end();
	user_positions.fill( ofVec2f( .0f ) ); // refilling array every call might take a while, maybe just handle the lostUser event smarter
	auto i = 0;
	while (it != itEnd) {
		float xl = it->second.left().x * width;
		float yl = it->second.left().y * height;
		float xr = it->second.right().x * width;
		float yr = it->second.right().y * height;

		ofVec2f left = getProjectedPosition( ofVec3f( xl, yl, 0.f ) );
		ofVec2f right = getProjectedPosition( ofVec3f( xr, yr, 0.f ) );

		user_positions[i++] = left;
		user_positions[i++] = right;

		it++;
	}
}

bool ccScene::isInBounds( ofVec2f pos )
{
	return isInBounds( pos.x, pos.y );
}

bool ccScene::isInBounds( int x, int y ) {
	return ((x > 0 && x < width) && (y > 0 && y < height));
}

filesystem::path ccScene::getCurrentPath()
{
	return scenesPath / getName();
}

filesystem::path ccScene::getShaderPath() {
	return getCurrentPath() / "shader";
}

// Not called
void ccScene::keyPressed( int key ) {

	// std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
	}
	else if (key == 'r' || key == 'R') {
		resetCamera();
	}
}

void ccScene::keyReleased( int key ) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();
	}
}

void ccScene::mousePressed( int x, int y, int button )
{
	mouseIsDown = true;
	mousePosition.set( getProjectedPosition( ofVec3f( x, y, 0.0 ) ) );
}

void ccScene::mouseReleased( int x, int y, int button )
{
	mouseIsDown = false;
}

void ccScene::mouseDragged( int x, int y, int button )
{
		mousePosition.set( getProjectedPosition( ofVec3f( x, y, 0.0 ) ) );
}

void ccScene::windowResized( int w, int h ) {
	width = w;
	height = h;
}

vector<ofVec3f> ccScene::getHandsWorldCoords() {
	vector<ofVec2f> hands_camera = userManager->getHandsVec();

	vector<ofVec3f> hands_world;
	for (auto& h : hands_camera) {
		ofVec3f hand = ofVec3f(h.x, h.y, 0.0);
		hands_world.push_back(getNormalToWorldPosition(hand));
	}

	return hands_world;
}

// Triggered when this scene is opened
float ccScene::SceneIntro() {
	cout << "Default Scene Intro Triggered" << endl;
	return 1.f;
}

// Triggered when this scene is closed
float ccScene::SceneOutro() {
	cout << "Default Scene Outro Triggered" << endl;
	return 1.f;
}