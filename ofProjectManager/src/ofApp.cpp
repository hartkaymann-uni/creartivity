#include "ofApp.h"

#include "scenes/scenes.h"

using namespace gol;

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground( 255, 255, 0 );

	ofSetFrameRate( 60 );
	ofSetVerticalSync( false );

	transformer.setRenderSize( 1024, 768 );
	transformer.setTransforms( true, true, false, true, true );
	setTransformer( &transformer );

	receiver.setUserManager( &userManager );

	// Load scenes
	//scenes.push_back( (SpiralScene*)sceneManager.add( new SpiralScene() ) );
	scenes.push_back( (FluidScene*)sceneManager.add( new FluidScene() ) );
	scenes.push_back( (GameOfLifeScene*)sceneManager.add( new GameOfLifeScene() ) );
	scenes.push_back( (SwarmScene*)sceneManager.add( new SwarmScene() ) );
	scenes.push_back( (ContourLinesScene*)sceneManager.add( new ContourLinesScene() ) );

	// Initialize scene manager
	sceneManager.setup( true ); // Setup all scenes now
	ofSetLogLevel( "ofxScenemanager", OF_LOG_VERBOSE );
	sceneManager.gotoScene( "Fluid", true );
	lastScene = sceneManager.getCurrentSceneIndex();
	sceneManager.setOverlap( false );
	nextAction = NULL;

	setSceneManager( &sceneManager );

	// Give all scenes a pointer to the receiver
	// TODO: Scenen dont need this anymore, as user array does the work here
	for (ccScene* scene : scenes) {
		scene->setUserManager( &userManager );
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	receiver.receiveMessages();

	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	//
	CheckSceneTransitions();
}

//--------------------------------------------------------------
void ofApp::draw() {

	if (isDebug()) {
		ofNoFill();
		ofSetColor( 255 );
		ofSetRectMode( OF_RECTMODE_CORNER );
		ofDrawRectangle( 1, 1, getRenderWidth() - 2, getRenderHeight() - 2 );
		ofFill();
	}

	transformer.pop();

	// Draw current scene info
	ofSetColor( 200 );
	ofxBitmapString( 12, ofGetHeight() - 8 )
		<< "Current Scene: " << sceneManager.getCurrentSceneIndex()
		<< " " << sceneManager.getCurrentSceneName() << endl;

	if (showGui) {
		for (ccScene* s : scenes) {
			if (s->getName() == sceneManager.getCurrentSceneName()) {
				ofxPanel& gui = s->getGui();
				gui.setPosition( ofGetWidth() - gui.getWidth() - 10, ofGetHeight() - gui.getHeight() - 10 );
				s->getGui().draw();
			}
		}
	}

	transformer.push();
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key ) {
	switch (key) {

	case 'd':
		//bDebug = !bDebug;
		break;
	case 'h':
		showGui = !showGui;
		break;
	case 'c':
		ofShowCursor();
		break;

	case 'f':
		ofToggleFullscreen();
		break;

	case OF_KEY_LEFT:
		ChangeScene( SceneChangeType::Previous );
		break;

	case OF_KEY_RIGHT:
		ChangeScene( SceneChangeType::Next );
		break;

	case OF_KEY_DOWN:
		if (sceneManager.getCurrentScene()) { // returns NULL if no scene selected
			lastScene = sceneManager.getCurrentSceneIndex();
		}
		sceneManager.noScene();
		break;

	case OF_KEY_UP:
		sceneManager.gotoScene( lastScene );
		break;

	case 'o':
		sceneManager.setOverlap( !sceneManager.getOverlap() );
		break;
	}
}

//--------------------------------------------------------------
void ofApp::CheckSceneTransitions() {
	if (ofGetElapsedTimef() > nextActionTime && nextAction != NULL) {
		(this->*nextAction)();
		nextAction = NULL;
	}
}


//--------------------------------------------------------------
void ofApp::ChangeScene( SceneChangeType type ) {
	unsigned int currentSceneIndex = sceneManager.getCurrentSceneIndex();
	float delay = 0.f;
	// 'scenes' is not in the same order as the array that 'sceneManager' uses. Therefore we can't use the 'currentSceneIndex'
	// from 'sceneManager' for the 'scenes' array.
	if (sceneManager.getCurrentScene() != NULL) {
		delay = static_cast<ccScene*>(sceneManager.getCurrentScene())->SceneOutro();
	}

	switch (type)
	{
	case SceneChangeType::Next:
		nextAction = &ofApp::NextScene;
		break;
	case SceneChangeType::Previous:
		nextAction = &ofApp::PreviousScene;
		break;
	default:
		break;
	}

	delay = max( 0.f, delay );
	nextActionTime = ofGetElapsedTimef() + delay;
}

//--------------------------------------------------------------
void ofApp::NextScene() {
	int nextSceneIndex = GetSceneIndex( SceneChangeType::Next );
	sceneManager.nextScene();
	if (sceneManager.getSceneAt( nextSceneIndex ) != NULL) {
		static_cast<ccScene*>(sceneManager.getSceneAt( nextSceneIndex ))->SceneIntro();
	}
}

//--------------------------------------------------------------
unsigned int  ofApp::GetSceneIndex( SceneChangeType type ) {
	unsigned int currentSceneIndex = sceneManager.getCurrentSceneIndex();


	switch (type)
	{
	case SceneChangeType::Next:
		if (currentSceneIndex >= sceneManager.getNumScenes() - 1) {
			return 0;
		}
		else {
			return currentSceneIndex + 1;
		}
		break;
	case SceneChangeType::Previous:
		if (currentSceneIndex <= 0) {
			return sceneManager.getNumScenes() - 1;
		}
		else {
			return currentSceneIndex - 1;
		}
		break;
	default:
		return -1;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::PreviousScene() {
	int previousIndex = GetSceneIndex( SceneChangeType::Previous );
	sceneManager.prevScene();
	if (sceneManager.getSceneAt( previousIndex ) != NULL) {
		static_cast<ccScene*>(sceneManager.getSceneAt( previousIndex ))->SceneIntro();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key ) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button ) {
	// Left click is left mouse position, right click right mouse position
	ccUser* mouse = userManager.getMouseUser();
	if (button == OF_MOUSE_BUTTON_LEFT)
		mouse->setPositions( { (x * 1.f) / ofGetWidth(), (y * 1.f) / ofGetHeight(), 0.f }, mouse->right() );
	else
		mouse->setPositions( mouse->left(), { (x * 1.f) / ofGetWidth(), (y * 1.f) / ofGetHeight(), 0.f } );
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button ) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button ) {
	// Reset user motion by moving by zero 
	ccUser* mouse = userManager.getMouseUser();
	if (button == OF_MOUSE_BUTTON_LEFT)
		mouse->setMotions( { 0.f, 0.f, 0.f }, mouse->getMotions().second );
	else
		mouse->setMotions( mouse->getMotions().first, { 0.f, 0.f, 0.f } );
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h ) {
	// transformer.setNewScreenSize() is automatically called if the transformer is set
	for (ccScene* scene : scenes) {
		scene->windowResized( w, h );
	}
}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg ) {

}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo ) {

}
