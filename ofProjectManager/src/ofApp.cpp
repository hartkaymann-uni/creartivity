#include "ofApp.h"

#include "scenes/scenes.h"

#define SWITCH_SCENES

using namespace gol;
using namespace contour;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;


void ofApp::setup() {
	ofBackground( 255, 255, 0 );

	ofSetFrameRate( 60 );
	ofSetVerticalSync( false );

	/*transformer.setRenderSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	transformer.setTransforms( true, true, false, true, true );
	setTransformer( &transformer );*/

	receiver = ccReceiver( HOST, PORT );

	// Load scenes
	scenes.push_back( (FluidScene*)sceneManager.add( new FluidScene() ) );
	scenes.push_back( (GameOfLifeScene*)sceneManager.add( new GameOfLifeScene( SCREEN_WIDTH / 20, SCREEN_HEIGHT / 20 ) ) );
	scenes.push_back( (SwarmScene*)sceneManager.add( new SwarmScene() ) );
	scenes.push_back( (ContourLinesScene*)sceneManager.add( new ContourLinesScene( SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 ) ) );

	// Initialize scene manager
	sceneManager.setup( true ); // Setup all scenes now
	ofSetLogLevel( "ofxScenemanager", OF_LOG_VERBOSE );
	sceneManager.gotoScene( "GameOfLife", true );

	lastScene = sceneManager.getCurrentSceneIndex();
	sceneManager.setOverlap( false );
	nextAction = NULL;

	setSceneManager( &sceneManager );
}

void ofApp::update() {
	ccUserManager& um = ccUserManager::get();
	um.updateUserPositions();

#ifdef SWITCH_SCENES
	// Change to next scene at an intervall
	float time = ofGetElapsedTimef();
	if ( time - lastSceneChangeTime > durationPerScene ) {
		lastSceneChangeTime = time;
		ChangeScene( SceneChangeType::Next );
	}
#endif
	receiver.receiveMessages();

	// Display framerate in window title
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle( strm.str() );

	CheckSceneTransitions();
}

void ofApp::draw() {

	// Debug gui
	if ( isDebug() ) {
		ofNoFill();
		ofSetColor( 255 );
		ofSetRectMode( OF_RECTMODE_CORNER );
		ofDrawRectangle( 1, 1, getRenderWidth() - 2, getRenderHeight() - 2 );
		ofFill();
	}

	// Draw current scene info
	//transformer.pop();
	ofSetColor( 200 );
	if ( showGui ) {
		ofxBitmapString( 12, ofGetHeight() - 8 )
			<< "Current Scene: " << sceneManager.getCurrentSceneIndex()
			<< " " << sceneManager.getCurrentSceneName() << endl;

		for ( ccScene* s : scenes ) {
			if ( s->getName() == sceneManager.getCurrentSceneName() ) {
				ofxPanel& gui = s->getGui();
				gui.setPosition( ofGetWidth() - gui.getWidth() - 10, ofGetHeight() - gui.getHeight() - 10 );
				s->getGui().draw();
			}
		}
	}
	//transformer.push();
}

void ofApp::keyPressed( int key ) {
	switch ( key ) {

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
		if ( sceneManager.getCurrentScene() ) { // returns NULL if no scene selected
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

	case 'x':
		// Take a screenshot
		img.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
		string filename = "screenshots/" + ofGetTimestampString( "%Y-%m-%d-%H-%M-%S-%i" ) + ".png";
		img.save( filename );
		break;
	}
}

// Check wether time for the next scene has come and do scene transition if it has
void ofApp::CheckSceneTransitions() {
	if ( ofGetElapsedTimef() > nextActionTime && nextAction != NULL ) {
		(this->*nextAction)();
		nextAction = NULL;
	}
}

/// <summary>
/// Change scene into specified direction. Delay before transition is determined by return value of SceneOutro.
/// </summary>
/// <param name="type">Type of transition, either Next or Previous.</param>
void ofApp::ChangeScene( SceneChangeType type ) {
	unsigned int currentSceneIndex = sceneManager.getCurrentSceneIndex();
	float delay = 0.f;
	// 'scenes' is not in the same order as the array that 'sceneManager' uses. Therefore we can't use the 'currentSceneIndex'
	// from 'sceneManager' for the 'scenes' array.
	if ( sceneManager.getCurrentScene() != NULL ) {
		delay = static_cast<ccScene*>(sceneManager.getCurrentScene())->SceneOutro();
	}

	switch ( type )
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

void ofApp::NextScene() {
	int nextSceneIndex = GetSceneIndex( SceneChangeType::Next );
	sceneManager.nextScene();
	if ( sceneManager.getSceneAt( nextSceneIndex ) != NULL ) {
		static_cast<ccScene*>(sceneManager.getSceneAt( nextSceneIndex ))->SceneIntro();
	}
}

void ofApp::PreviousScene() {
	int previousIndex = GetSceneIndex( SceneChangeType::Previous );
	sceneManager.prevScene();
	if ( sceneManager.getSceneAt( previousIndex ) != NULL ) {
		static_cast<ccScene*>(sceneManager.getSceneAt( previousIndex ))->SceneIntro();
	}
}

unsigned int  ofApp::GetSceneIndex( SceneChangeType type ) {
	unsigned int currentSceneIndex = sceneManager.getCurrentSceneIndex();

	switch ( type )
	{
	case SceneChangeType::Next:
		if ( currentSceneIndex >= sceneManager.getNumScenes() - 1 ) {
			return 0;
		}
		else {
			return currentSceneIndex + 1;
		}
		break;
	case SceneChangeType::Previous:
		if ( currentSceneIndex <= 0 ) {
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

void ofApp::mouseDragged( int x, int y, int button ) {
	// Left click is left mouse position, right click right mouse position
	ccUserManager& um = ccUserManager::get();
	ccUser* mouse = um.getMouseUser();
	if ( mouse == NULL )
		return;

	// Left mouse button is "left hand", right mouse button is "right hand"
	if ( button == OF_MOUSE_BUTTON_LEFT )
		mouse->setPositions( { (x * 1.f) / ofGetWidth(), (y * 1.f) / ofGetHeight(), 0.f }, mouse->right() );
	else if ( button == OF_MOUSE_BUTTON_RIGHT )
		mouse->setPositions( mouse->left(), { (x * 1.f) / ofGetWidth(), (y * 1.f) / ofGetHeight(), 0.f } );
}

void ofApp::mousePressed( int x, int y, int button ) {
	ccUserManager& um = ccUserManager::get();
	if ( um.getMouseUser() == NULL )
		um.registerMouseUser();
}

void ofApp::mouseReleased( int x, int y, int button ) {
	ccUserManager& um = ccUserManager::get();
	um.removeMouseUser();
}

void ofApp::windowResized( int w, int h ) {
	// transformer.setNewScreenSize() is automatically called if the transformer is set
	for ( ccScene* scene : scenes ) {
		scene->windowResized( w, h );
	}
}