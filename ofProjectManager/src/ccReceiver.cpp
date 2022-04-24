#include "ccReceiver.h"

ccReceiver::ccReceiver( string host, int port )
{
	receiver.setup( PORT );
	ofLog() << "Listening on port " << PORT;
}

// Receive and handle user data sent by our ofInput
// TODO: create smaller functions to handle different adresses
void ccReceiver::receiveMessages() {
	ofxOscMessage m;
	while (receiver.hasWaitingMessages()) {
		receiver.getNextMessage( &m );

		string address = m.getAddress();
		string connectionAdr = "/connection";
		string userAdr = "/user";
		if (address.find( userAdr ) != string::npos) {
			if (address.find( "data/" ) != string::npos) {

				string idUs = address.substr( 11 );
				int id = ofToInt( idUs );

				float xl = m.getArgAsFloat( 0 );
				float yl = m.getArgAsFloat( 1 );
				float xr = m.getArgAsFloat( 2 );
				float yr = m.getArgAsFloat( 3 );

				userManager->getUser( id )->setPositions( glm::vec3( xl, yl, 0.f ), glm::vec3( xr, yr, 0.f ) );

				ccUser* u = userManager->getUser( id );
				//printf( "user %i: Left:[ %.3f, %.3f] Right:[ %.3f, %.3f ] \n", id, u->left().x, u->left().y, u->right().x, u->right().y );
			}
			else if (address.find( "new/" ) != string::npos) {
				int id = m.getArgAsInt32( 0 );
				ccUser newUser{ id, glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, 0.f ) };
				userManager->registerUser( newUser );

				ofLog( ofLogLevel::OF_LOG_NOTICE, "New User: %i", ofToString( id ) );
			}
			else if (address.find( "lost/" ) != string::npos) {
				int id = m.getArgAsInt32( 0 );
				userManager->removeUser( id );

				ofLog( ofLogLevel::OF_LOG_NOTICE, "Lost User: %i", ofToString( id ) );
			}
		}
		else if (address.find( connectionAdr ) != string::npos) {
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
			ofLog( ofLogLevel::OF_LOG_WARNING, ss.str() );
		}
		m.clear();
	}
}