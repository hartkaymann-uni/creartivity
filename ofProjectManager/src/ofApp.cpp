#include "ofApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground( 255, 255, 0 );

	ofSetFrameRate( 60 );
	ofSetVerticalSync( false );

	transformer.setRenderSize( 1024, 768 );
	transformer.setTransforms( true, true, false, true, true );
	setTransformer( &transformer );

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
	setSceneManager( &sceneManager );

	// Give all scenes a pointer to the receiver
	// TODO: Scenen dont need this anymore, as user array does the work here
	for (ccScene* scene : scenes) {
		scene->setReceiver( &receiver );
		scene->setUserManager( &userManager);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	receiver.receiveMessages();

	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );
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

	for (ccScene* s : scenes) {
		if (s->getName() == sceneManager.getCurrentSceneName()) {
			s->getGui().draw();
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

	case 'c':
		ofShowCursor();
		break;

	case 'f':
		ofToggleFullscreen();
		break;

	case OF_KEY_LEFT:
		sceneManager.prevScene();
		break;

	case OF_KEY_RIGHT:
		sceneManager.nextScene();
		break;

	case OF_KEY_DOWN:
		if (sceneManager.getCurrentScene()) { // returns NULL if no scene selected
			lastScene = sceneManager.getCurrentSceneIndex();
		}
		sceneManager.noScene();
		break;
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
	userManager.getMouseUser()->setPosition( { x, y } );
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button ) {
	userManager.getMouseUser()->setPosition( { x, y } );
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button ) {
	// Reset user motion by moving by zero 
	userManager.getMouseUser()->move( { 0.f, 0.f, 0.f } );
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
