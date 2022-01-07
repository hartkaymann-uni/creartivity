#include "ofApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync( true );
	ofBackground( 0 );

	transformer.setRenderSize( 1024, 768 );
	transformer.setTransforms( true, true, false, true, true );
	setTransformer( &transformer );

	// Load scenes
	//scenes.push_back( (ParticleScene*)sceneManager.add( new ParticleScene() ) );
	scenes.push_back( (IntroScene*)sceneManager.add( new IntroScene() ) );
	scenes.push_back( (GameOfLifeScene*)sceneManager.add( new GameOfLifeScene() ) );
	scenes.push_back( (CuboidScene*)sceneManager.add( new CuboidScene() ) );
	scenes.push_back( (SwarmScene*)sceneManager.add( new SwarmScene() ) );
	scenes.push_back( (ContourLinesScene*)sceneManager.add( new ContourLinesScene() ) );
	sceneManager.setup( true ); // Setup all scenes now
	ofSetLogLevel( "ofxScenemanager", OF_LOG_VERBOSE );

	sceneManager.gotoScene( "Intro", true );
	lastScene = sceneManager.getCurrentSceneIndex();
	sceneManager.setOverlap( false );

	setSceneManager( &sceneManager );

	for (ccScene* scene : scenes) {
		scene->setReceiver( &receiver );
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
		bDebug = !bDebug;
		break;

	case 'a':
		transformer.setAspect( !transformer.getAspect() );
		break;

	case 'c':
		transformer.setCentering( !transformer.getCentering() );
		break;

	case 'm':
		transformer.setMirrorX( !transformer.getMirrorX() );
		break;

	case 'n':
		transformer.setMirrorY( !transformer.getMirrorY() );
		break;

	case 'q':
		transformer.setWarp( !transformer.getWarp() );
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

	case OF_KEY_UP:
		sceneManager.gotoScene( lastScene );
		break;

	case 'o':
		sceneManager.setOverlap( !sceneManager.getOverlap() );
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

}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button ) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button ) {

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
