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

				users[id].positionLeft.x = xl != xl ? 0.f : xl;
				users[id].positionLeft.y = yl != yl ? 0.f : yl;
				users[id].positionRight.x = xr != xr ? 0.f : xr;
				users[id].positionRight.y = yr != yr ? 0.f : yr;

				user* u = &users[id];
				printf( "user %i: Left:[ %.3f, %.3f] Right:[ %.3f, %.3f ] \n", id, u->positionLeft.x, u->positionLeft.y, u->positionRight.x, u->positionRight.y );
			}
			else if (address.find( "new/" ) != string::npos) {
				int id = m.getArgAsInt32( 0 );
				user newUser{ id, {0.0, 0.0},{0.0, 0.0} };
				users[id] = newUser;
				ofLog( ofLogLevel::OF_LOG_NOTICE, "New User: %i", ofToString( id ) );
			}
			else if (address.find( "lost/" ) != string::npos) {
				int id = m.getArgAsInt32( 0 );
				users.erase( id );
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