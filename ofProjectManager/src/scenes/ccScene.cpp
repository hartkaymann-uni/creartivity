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
void ccScene::receiveMessage() {
	if (receiver == nullptr)
		return;

	ofxOscMessage m;
	while (receiver->hasWaitingMessages()) {
		receiver->getNextMessage( &m );

		string address = m.getAddress();
		string connectionAdr = "/connection";
		string userAdr = "/user/data/";
		if (address.find( userAdr ) != string::npos) {
			string idUs = address.substr( 11 );
			int id = ofToInt( idUs );


			float xl = m.getArgAsFloat( 0 );
			float yl = m.getArgAsFloat( 1 );
			float xr = m.getArgAsFloat( 2 );
			float yr = m.getArgAsFloat( 3 );

			// Insert new user
			if (users.find( id ) == users.end()) {
				user newUser{ id, {xl, yl},{xr, yr} };
				users[id] = newUser;
			}
			else {
				users[id].positionLeft.x = xl != xl ? 0.f : xl;
				users[id].positionLeft.y = yl != yl ? 0.f : yl;
				users[id].positionRight.x = xr != xr ? 0.f : xr;
				users[id].positionRight.y = yr != yr ? 0.f : yr;
			}

			user* u = &users[id];
			printf( "user %i: Left:[ %.3f, %.3f] Right:[ %.3f, %.3f ] \n", id, u->positionLeft.x, u->positionLeft.y, u->positionRight.x, u->positionRight.y );
		}
		else if (address.find( connectionAdr ) != string::npos){
			string status = m.getArgAsString( 0 );
			string host = m.getArgAsString( 1 );
			int port = m.getArgAsInt32( 2 );

			stringstream ss;
			ss << "Status: " << status << " Host: " << host << " Port: " << ofToString( port );

			connectionStatusString = ss.str();
		}
		else {
			stringstream ss;
			ss << "Can't read message to address: " << address;
			ofLogError( ss.str() );
		}
		m.clear();
	}
}

void ccScene::resetCamera()
{
	camera.reset();
	camera.setPosition( width / 2, height / 2, 665 );
}

ofVec3f ccScene::getProjectedPosition( ofVec3f p ) {

	glm::vec3 pos = camera.screenToWorld( p );

	pos.z = 0.0;

	std::cout << "Projected: X=[ " << pos.x << " ] Y=[ " << pos.y << " ] Z=[ " << pos.z << " ]" << std::endl;

	return ofVec3f( pos );
}

