#include "ccScene.h"

ccScene::ccScene( std::string name )
	: ofxScene( name ),
	width( ofGetWidth() ),
	height( ofGetHeight() ),
	mouseIsDown( false ),
	mousePosition( 0.f, 0.f, 0.f ),
	receiver( nullptr ),
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

void ccScene::setup()
{
	stringstream ss;
	ss << "Called setup of" << this->getName() << std::endl;
	ofLog( ofLogLevel::OF_LOG_WARNING, ss.str() );
}

void ccScene::update()
{
	stringstream ss;
	ss << "Called update of" << this->getName() << std::endl;
	ofLog( ofLogLevel::OF_LOG_WARNING, ss.str() );
}

void ccScene::draw()
{
	stringstream ss;
	ss << "Called draw of" << this->getName() << std::endl;
	ofLog( ofLogLevel::OF_LOG_WARNING, ss.str() );
}

void ccScene::resetCamera()
{
	camera.reset();
	camera.setPosition( width / 2, height / 2, (width + height) / 2 );
}

ofVec3f ccScene::getProjectedPosition( ofVec3f p ) {

	glm::vec3 pos = camera.screenToWorld( p );

	pos.z = 0.0;

	//std::cout << "Projected: X=[ " << pos.x << " ] Y=[ " << pos.y << " ] Z=[ " << pos.z << " ]" << std::endl;

	return ofVec3f( pos );
}

void ccScene::updateUserPositions()
{
	unique_ptr<map<int, user>> users = receiver->getUsers();
	std::map<int, user>::iterator it = users->begin();
	std::map<int, user>::iterator itEnd = users->end();
	user_positions.fill( ofVec2f( .0f ) ); // refilling array every call might take a while, maybe just handle the lostUser event smarter
	auto i = 0;
	while (it != itEnd) {
		float xl = it->second.positionLeft.x * width;
		float yl = it->second.positionLeft.y * height;
		float xr = it->second.positionRight.x * width;
		float yr = it->second.positionRight.y * height;

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
	if (mouseIsDown)
		mousePosition.set( getProjectedPosition( ofVec3f( x, y, 0.0 ) ) );
}

void ccScene::windowResized( int w, int h ) {
	width = w;
	height = h;
}