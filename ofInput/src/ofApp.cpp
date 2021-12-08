#include "ofApp.h"


void ofApp::setup()
{
	width = ofGetWidth();
	height = ofGetHeight();

	sender.setup( HOST, PORT );

	for (auto i = 0; i < NO_USERS; i++) {
		users[i].position = ofVec3f( 0.f );
	}
}


void ofApp::update() {

}



void ofApp::draw() {
	ofClear( 0 );
	ofBackground( 0 );
	int radius = 10;

	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		user* u = &(it->second);
		ofDrawCircle( (u->position.x * width), (u->position.y * height), radius );
		it++;
	}

}


void ofApp::keyPressed( int key ) {

}


void ofApp::keyReleased( int key ) {

}


void ofApp::mouseMoved( int x, int y ) {

}


void ofApp::mouseDragged( int x, int y, int button ) {

	for (auto i = 0; i < users.size(); i++)
	{
		users[i].position.x = ofMap( (x + 100 * i) % width, 0.f, (float)width, 0.f, 1.f );
		users[i].position.y = ofMap( (y + 100 * i) % height, 0.f, (float)height, 0.f, 1.f );
	}

	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		sendUser( it->first, it->second );
		it++;
	}
}

void ofApp::sendUser( int id, user& user ) {
	ofxOscMessage m;
	std::string addr = "/user/data/";
	addr += ofToString( id );

	string leftAdr = addr + "/left";
	m.setAddress( leftAdr);

	m.addFloatArg( user.position.x );
	m.addFloatArg( user.position.y );

	sender.sendMessage( m );
}


void ofApp::mousePressed( int x, int y, int button ) {

}


void ofApp::mouseReleased( int x, int y, int button ) {

}


void ofApp::mouseEntered( int x, int y ) {

}


void ofApp::mouseExited( int x, int y ) {

}


void ofApp::windowResized( int w, int h ) {
	width = w;
	height = h;
}


void ofApp::gotMessage( ofMessage msg ) {

}


void ofApp::dragEvent( ofDragInfo dragInfo ) {

}
