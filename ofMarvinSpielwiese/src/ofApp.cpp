#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	bool pause = false;
	ofSetFrameRate(60);
	int maxDistance = 200;
	bool lineIsEnabled = false;
	bool lightningIsEnabled = false;
	bool curveIsEnabled = false;
	ofBackground(0);

}

//--------------------------------------------------------------
void ofApp::update() {

	// Opportunity to pause
	if (isPaused) return;

	// Update or delete existing nodes
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].isVisible())
		{
			nodes[i].update();
		}
		else
		{
			nodes.erase(nodes.begin() + i);
		}
	}

	// Generate some random nodes
	while (nodes.size() < 512)
	{
		Node newHub(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		nodes.push_back(newHub);
	}
}


//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	ofSetLineWidth(1);

	// Draw circles for every node
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].draw();
	}

	// Draw lines or lightning between nodes
	for (int i = 0; i < nodes.size() - 1; i++)
	{
		glm::vec2 positionOne = nodes[i].position;
		for (int j = i + 1; j < nodes.size(); j++)
		{
			glm::vec2 positionTwo = nodes[j].position;
			int distance = glm::distance(positionOne, positionTwo);
			if (distance < maxDistance)
			{
				if (curveIsEnabled)drawCurves(i, j, distance);
				if (lineIsEnabled) ofDrawLine(positionOne, positionTwo);
				if (lightningIsEnabled) drawLightning(positionOne, positionTwo, distance / 3);
			}
		}
	}

}

void ofApp::drawCurves(int i, int j, float distance) {
	float x1 = nodes[i].position.x;
	float y1 = nodes[i].position.y;
	float x4 = nodes[j].position.x;
	float y4 = nodes[j].position.y;
	float x2 = (x1 + x4) / 2 - distance / 2 * nodes[i].direction.x;
	float y2 = (y1 + y4) / 2 - distance / 2 * nodes[i].direction.y;
	float x3 = (x1 + x4) / 2 - distance / 2 * nodes[j].direction.x;
	float y3 = (y1 + y4) / 2 - distance / 2 * nodes[j].direction.y;

	ofDrawBezier(x1, y1, x2, y2, x3, y3, x4, y4);
}

void ofApp::drawLightning(glm::vec2 Pos1, glm::vec2 Pos2, float displace) {

	if (displace <= 1) ofDrawLine(Pos1, Pos2);
	else
	{
		float mid_x = (Pos1.x + Pos2.x) / 2 + (ofRandom(1) - 0.5) * displace;
		float mid_y = (Pos1.y + Pos2.y) / 2 + (ofRandom(1) - 0.5) * displace;

		mid_x += (ofRandom(1) - 0.5) * displace;
		mid_y += (ofRandom(1) - 0.5) * displace;

		glm::vec2 mid = glm::vec2(mid_x, mid_y);

		drawLightning(Pos1, mid, displace / 2);
		drawLightning(Pos2, mid, displace / 2);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'f') ofToggleFullscreen();
	if (key == 'p') isPaused = !isPaused;
	if (key == 'w') maxDistance += 10;
	if (key == 's') maxDistance -= 10;
	if (key == '1') lineIsEnabled = !lineIsEnabled;
	if (key == '2') lightningIsEnabled = !lightningIsEnabled;
	if (key == '3') curveIsEnabled = !curveIsEnabled;
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	Node newNode(mouseX, mouseY);
	nodes.push_back(newNode);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
Node::Node(int startX, int startY) {
	float directionSpeed = 1.0;
	position = glm::vec2(startX, startY);
	direction = glm::vec2(ofRandom(-directionSpeed, directionSpeed), ofRandom(-directionSpeed, directionSpeed));
	size = 20;
}

Node::~Node() {

}

bool Node::isVisible() {
	int overlap = 20;
	return(position.x > -overlap &&
		position.x < ofGetWidth() + overlap &&
		position.y > -overlap &&
		position.y < ofGetHeight() + overlap &&
		size > 1);
}

void Node::update() {
	float time = ofGetElapsedTimef();
	position += direction;
	size += ofRandom(-0.02, 0.001);
}

void Node::draw() {
	ofNoFill();
	//ofDrawCircle(position, size);
}