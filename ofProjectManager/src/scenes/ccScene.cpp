#include "ccScene.h"

ccScene::ccScene( std::string name )
	: ofxScene( name ),
	width( ofGetWidth() ),
	height( ofGetHeight() ),
	receiver( nullptr )
{
	setSingleSetup( true );

	camera.disableMouseInput();
	camera.enableOrtho();
	camera.setPosition( ofGetWidth() / 2, ofGetHeight() / 2, 665 );
	camera.setNearClip( -10 * ofGetWidth() );
	camera.setFarClip( ofGetWidth() * 10 );
}

// Receive and handle user data sent by our ofInput
void ccScene::receiveUsers() {
	if (receiver == nullptr)
		return;

	ofxOscMessage m;
	while (receiver->hasWaitingMessages()) {
		receiver->getNextMessage( &m );

		string address = m.getAddress();
		string userAdr = "/user/data/";
		int found = address.find( userAdr );
		if (found != string::npos) {
			string idUs = address.substr( 11 );
			int id = ofToInt( idUs );


			float xl = m.getArgAsFloat( 0 );
			float yl = m.getArgAsFloat( 1 );
			float xr = m.getArgAsFloat( 2 );
			float yr = m.getArgAsFloat( 3 );

			printf( "user %i: Left:[ %.3f, %.3f] Right:[ %.3f, %.3f ] \n", id, xl, yl, xr, yr );

			users[id].positionLeft.x = xl;
			users[id].positionLeft.y = yl;
			users[id].positionRight.x = xr;
			users[id].positionRight.y = yr;
		}
		m.clear();
	}
}

void ccScene::resetCamera()
{
	camera.reset();
	camera.setPosition( width / 2, height / 2, 665 );
}

ofVec3f ccScene::getProjectedMousePosition( ofVec3f mp ) {

	glm::vec3 pos = camera.screenToWorld( mp );

	pos.z = 0.0;

	std::cout << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << std::endl;

	return ofVec3f( pos );
}

