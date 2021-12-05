#include "ccScene.h"

ccScene::ccScene( std::string name )
	: ofxScene( name ),
	width( ofGetWidth() ),
	height( ofGetHeight() ),
	receiver( nullptr )
{
	setSingleSetup( true );

}

void ccScene::receiveMessage() {
	if (receiver == nullptr) 
		return;
	
	while (receiver->hasWaitingMessages()) {
		ofxOscMessage m;
		receiver->getNextMessage( &m );

		if (m.getAddress() == "/mouse/position")
		{
			blobsXf = m.getArgAsFloat( 0 );
			blobsYf = m.getArgAsFloat( 1 );
		}
		std::cout << "X: " << blobsXf << " Y: " << blobsYf << std::endl;
	}
}


