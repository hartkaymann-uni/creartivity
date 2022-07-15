#include "ccScene.h"

ccScene::ccScene( std::string name )
	: ofxScene( name ),
	width( ofGetWidth() ),
	height( ofGetHeight() ),
	mouseIsDown( false ),
	mousePosition( 0.f, 0.f, 0.f ),
	scenesPath( "../../src/scenes" ),
	userManager( new ccUserManager() )
{
	setSingleSetup(true);
	camera.disableMouseInput();
	camera.enableOrtho();
	camera.setPosition(width / 2, height / 2, (width + height) / 2);
	camera.setNearClip(-10 * ofGetWidth());
	camera.setFarClip(ofGetWidth() * 10);


	gui.setup();
}

void ccScene::resetCamera()
{
	camera.reset();
	camera.setPosition(width / 2, height / 2, (width + height) / 2);
}

ofVec3f ccScene::getProjectedPosition(ofVec3f p) {
	glm::vec3 pos = camera.screenToWorld(p);
	pos.z = 0.0;
	return ofVec3f(pos);
}

ofVec3f ccScene::getNormalToScreenPosition(ofVec2f normalPos) {
	ofVec2f preparedNormalPos = ofVec2f(min(max(normalPos.x, 0.f), 1.f), min(max(normalPos.y, 0.f), 1.f));

	ofVec2f screenPos = ofVec2f(preparedNormalPos.x * ofGetWidth(), preparedNormalPos.y * ofGetHeight());

	return screenPos;
}

ofVec3f ccScene::getScreenToWorldPosition(ofVec2f screenPos) {
	glm::vec3 preparedScreenPos = glm::vec3(screenPos.x, screenPos.y, 0);
	glm::vec3 worldPos = camera.screenToWorld(preparedScreenPos);

	return worldPos;
}

ofVec3f ccScene::getNormalToWorldPosition(ofVec2f normalPos) {
	ofVec2f screenPos = getNormalToScreenPosition(normalPos);
	ofVec2f worldPos = getScreenToWorldPosition(screenPos);

	return worldPos;
}

bool ccScene::isInBounds(ofVec2f pos)
{
	return isInBounds(pos.x, pos.y);
}

bool ccScene::isInBounds(int x, int y) {
	return ((x > 0 && x < width) && (y > 0 && y < height));
}

filesystem::path ccScene::getCurrentPath()
{
	return scenesPath / getName();
}

filesystem::path ccScene::getShaderPath() {
	return getCurrentPath() / "shader";
}

// Not called
void ccScene::keyPressed(int key) {

	// std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
		//std::cout << camera.getPosition() << std::endl;
	}
	else if (key == 'r' || key == 'R') {
		resetCamera();
	}
}

void ccScene::keyReleased(int key) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();
	}
}

void ccScene::mousePressed(int x, int y, int button)
{
	mouseIsDown = true;
	mousePosition.set(getProjectedPosition(ofVec3f(x, y, 0.0)));
}

void ccScene::mouseReleased(int x, int y, int button)
{
	mouseIsDown = false;
}

void ccScene::mouseDragged(int x, int y, int button)
{
	mousePosition.set(getProjectedPosition(ofVec3f(x, y, 0.0)));
}

void ccScene::windowResized(int w, int h) {
	width = w;
	height = h;
}

vector<ofVec3f> ccScene::getHandPositions(CoordinateSystem system) {
	vector<ofVec2f> hands_camera = userManager->getHandsVec();

	vector<ofVec3f> hands_world;
	for (auto& hand : hands_camera) {
		switch (system)
		{
		case ccScene::CoordinateSystem::Normals:
			hands_world.push_back(ofVec3f(hand.x, hand.y, 0.0));
			break;
		case ccScene::CoordinateSystem::Screen:
			hands_world.push_back(getNormalToScreenPosition(hand));
			break;
		case ccScene::CoordinateSystem::World:
			hands_world.push_back(getNormalToWorldPosition(hand));
			break;
		default:
			break;
		}
	}

	return hands_world;
}

// Triggered when this scene is opened
float ccScene::SceneIntro() {
	cout << "Default Scene Intro Triggered" << endl;
	return 1.f;
}

// Triggered when this scene is closed
float ccScene::SceneOutro() {
	cout << "Default Scene Outro Triggered" << endl;
	return 1.f;
}