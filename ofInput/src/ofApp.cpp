#include "ofApp.h"


void ofApp::setup()
{
	ofSetFrameRate( 60 );
	ofSetVerticalSync( false );
	ofBackground( 0 );

	width = ofGetWidth();
	height = ofGetHeight();

	sender.setup( HOST, PORT );

	sendConnectionStarted();

	gui.setup();
	gui.add( p_add.set( "Add user", false ) );
	gui.add( p_remove.set( "Remove user", false ) );
	gui.setPosition( width - gui.getWidth() - 1, height - gui.getHeight() - 10 );
	p_add.addListener( this, &ofApp::handleRegisterUser );
	p_add.addListener( this, &ofApp::handleRegisterUser );

	// Register first user
	registerUser( 0 );
}

void ofApp::update() {
	// Iterate through users and send information
	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		sendUser( it->first, it->second );
		it++;
	}
}

void ofApp::draw() {
	ofClear( 0 );
	ofBackground( 0 );
	int radius = 10;

	// Iterate through users and draw positions
	ofPushStyle();
	ofNoFill();
	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		ofDrawCircle( unmapped( it->second.left ), 10 );
		ofDrawCircle( unmapped( it->second.right ), 10 );
		it++;
	}
	ofPopStyle();
	gui.draw();
}

// Return normalized coordinates from screen coordinates
glm::vec2 ofApp::mapped( glm::vec2 v )
{
	glm::vec2 mapped;
	mapped.x = ofMap( v.x, 0.f, width, 0.f, 1.f, true );
	mapped.y = ofMap( v.y, 0.f, height, 0.f, 1.f, true );
	return mapped;
}

// Return screen coordinates from normalized coordinates
glm::vec2 ofApp::unmapped( glm::vec2 v )
{
	glm::vec2 unmapped;
	unmapped.x = ofMap( v.x, 0.f, 1.f, 0.f, width, true );
	unmapped.y = ofMap( v.y, 0.f, 1.f, 0.f, height, true );
	return unmapped;
}

// Send user information
void ofApp::sendUser( int id, user& user ) {
	ofxOscMessage m;
	std::string addr = "/user/data/";
	addr += ofToString( id );
	m.setAddress( addr );

	// Position left hand
	m.addFloatArg( user.left.x );
	m.addFloatArg( user.left.y );

	// Position rigt hand
	m.addFloatArg( user.right.x );
	m.addFloatArg( user.right.y );

	sender.sendMessage( m );
}

// Register a user
void ofApp::registerUser( int id )
{
	user u = { id, {0.f, 0.f}, {0.f, 0.f} };
	users[id] = u;

	ofxOscMessage m;
	m.setAddress( "/user/new/" );
	m.addInt32Arg( id );
	sender.sendMessage( m );
}

// Remove one user
void ofApp::removeUser( int id )
{
	map<int, user>::iterator it = users.find( id );
	users.erase( it );

	ofxOscMessage m;
	m.setAddress( "/user/lost/" );
	m.addInt32Arg( it->second.id );
	sender.sendMessage( m );
}

// Sent once upon establishing a connection
void ofApp::sendConnectionStarted() {
	ofxOscMessage m;
	m.setAddress( "/connection" );
	m.addStringArg( "on" );
	m.addStringArg( HOST );
	m.addInt32Arg( PORT );
	sender.sendMessage( m );
}

void ofApp::handleRegisterUser( bool& b )
{
	int id = users.size();
	registerUser( id );

	p_add.set( false );
}

void ofApp::handleRemoveUser( bool& b )
{
	int id = users.size();
	removeUser( id );

	p_add.set( false );
}

void ofApp::mouseDragged( int x, int y, int button ) {
	glm::vec2 coords = mapped( { x, y } );

	// Arrange users in circle pattern
	glm::vec2 center = { width * 0.5f, height * 0.5f };
	float radius = glm::length( center - glm::vec2( x, y ) );
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		users[i].left = coords + glm::vec2{0.1f * i};
		users[i].right = glm::vec2( 1.f ) - coords + glm::vec2{ 0.1f * i };
	}

}

void ofApp::mousePressed( int x, int y, int button ) {

}

void ofApp::mouseReleased( int x, int y, int button ) {

}

void ofApp::windowResized( int w, int h ) {
	width = w;
	height = h;
}