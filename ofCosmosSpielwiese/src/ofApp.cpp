#include "ofApp.h"
#include "DemoParticle.h"

void ofApp::setup()
{
	filesystem::path shader_path("../../res/shaders");

	particleShader.load(shader_path / "particle.vert", shader_path / "particle.frag");
	particleTrailShader.load(shader_path / "particleTail.vert", shader_path / "particleTail.frag");

	setUpParticles();
	isChargingImpulse = false;
	//particleSystem.setup();

	ofSetBackgroundAuto(false);
	gui.setup();
	gui.add(trail.setup("trail", 140, 0, 255));
	gui.add(demoParticle::scaleMod);
}

void ofApp::setUpParticles() {
	ofSetBackgroundColor(100, 0, 200);
	hue = 0;
}

void ofApp::update()
{
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	updateParticles();
	//particleSystem.update();

	std::cout << "particle count: " << particles.size() << std::endl;
	//std::cout << "MouseX: " << mouseX << "   MouseY: " << mouseY << std::endl;
}

void ofApp::draw()
{
	drawParticles();
	ofRectangle myRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	ofSetColor(0, 0, 0, trail);
	ofDrawRectangle(myRect);

	//particleSystem.draw();

	gui.draw();
}

void ofApp::drawParticles() {
	/*particleTrailShader.begin();
	for (int i = 0; i < particles.size();i++) {
		particleTrailShader.setUniform2f("particlePosition", particles[i].position);
		particles[i].drawTail();
	}
	particleTrailShader.end();*/

	// look through all the 'particle' objects in the 'particles' vector and instruct each one to 'draw' itself
	particleShader.begin();
	for (int i = 0; i < particles.size();i++) {
		particles[i].draw();
	}
	ofDrawCircle(200, 200, 100);
	particleShader.end();
}

void ofApp::updateParticles() {
	if (isPressingMouse)
	{
		particle newParticle(mouseX, mouseY, hue);
		particles.push_back(newParticle);
	}

	for (int i = 0; i < particles.size();i++) {
		if (particles[i].IsToBeDeleted()) {
			particles.erase(particles.begin() + i);
		}
		else {
			particles[i].update();
		}
	}
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'f':
	case 'F':
		ofToggleFullscreen();
		break;

	default:
		particleSystem.ChangeAttractionMode(key);
		break;
	}
}

void ofApp::mouseDragged(int x, int y, int button)
{
	updateMousePos(x, y);
}

void ofApp::mouseReleased(int x, int y, int button) {
	isPressingMouse = false;
	updateMousePos(x, y);

	if (isChargingImpulse) {
		glm::vec2 dir;
		if (mouseX != mouseXClick || mouseY != mouseYClick)
		{
			dir = glm::vec2(mouseX, mouseY) - glm::vec2(mouseXClick, mouseYClick);
		}
		else {
			dir = glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1));
		}
		
		particleSystem.Impulse(dir, glm::vec2(mouseXClick, mouseYClick));
		isChargingImpulse = false;
	}
}

void ofApp::mousePressed(int x, int y, int button)
{
	isPressingMouse = true;
	updateMousePos(x, y, "click");

	hue = ofRandom(255);
	switch (button) {
	case 0:
		particleSystem.ColorExplosion(ofColor(255, 0, 0, 50), glm::vec2(mouseX, mouseY));
		break;
	case 2:
		isChargingImpulse = true;
		break;
	default:
		break;
	}
}

void ofApp::updateMousePos(int x, int y, string action) {
	if (isInBounds(x, y)) 
	{
		mouseX = x;
		mouseY = y;
		if (action == "click") {
			mouseXClick = mouseX;
			mouseYClick = mouseY;
		}
	}
}