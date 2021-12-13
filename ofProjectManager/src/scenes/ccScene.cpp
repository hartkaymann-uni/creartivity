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
	camera.setPosition( width / 2, height / 2, 665 );
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

