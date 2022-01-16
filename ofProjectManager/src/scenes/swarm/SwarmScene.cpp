#include "SwarmScene.h"

#include "ofConstants.h"

SwarmScene::SwarmScene() : ccScene("Swarm") {

}

//--------------------------------------------------------------
void SwarmScene::setup() {
	//camera.setFarClip(ofGetWidth() * 10);
	ofShowCursor();

	particleGroups = 400;
	particleAmount = 1024 * particleGroups;

	particles.resize(std::max(particleAmount, 1024));
	int i = 0;
	for (auto& p : particles) {
		p.pos.x = ofRandom(0, 500);
		p.pos.y = ofRandom(0, 500);
		//p.pos.z = ofRandom(-ofGetHeight()*0.5,ofGetHeight()*0.5);
		p.pos.z = ofRandom(-10000, 0);
		p.pos.w = 1;
		p.vel = { 0,0,0,0 };
		p.unique = { 0, 0 ,0, 0 };
		p.color = { 1, 1, 1, 1 };
		p.initialPos = p.pos;
		p.bufferPos = p.pos;
		i++;
	}

	filesystem::path shader_path = getShaderPath();
	compute.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "swarm.comp");
	compute.linkProgram();

	colorSplash.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "colorSplash.comp");
	colorSplash.linkProgram();

	userEnter.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "swarmUserEnter.comp");
	userEnter.linkProgram();

	particleShader.load(shader_path / "swarm.vert", shader_path / "swarm.frag");

	particlesBuffer.allocate(particles, GL_DYNAMIC_DRAW);
	particlesBuffer2.allocate(particles, GL_DYNAMIC_DRAW);

	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particlesBuffer2.bindBase(GL_SHADER_STORAGE_BUFFER, 1);

	vbo.setVertexBuffer(particlesBuffer, 4, sizeof(Particle));
	vbo.setColorBuffer(particlesBuffer, sizeof(Particle), sizeof(glm::vec4) * 2);

	ofBackground(0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	shaderUniforms.setName("shader params");
	shaderUniforms.add(attractionCoeff.set("attraction", 0, 0, 1));
	gui.add(UseAttraction.set("use_attraction", true));
	shaderUniforms.add(cohesionCoeff.set("cohesion", 0.5, 0, 1));
	gui.add(UseCohesion.set("use_cohesion", true));
	shaderUniforms.add(repulsionCoeff.set("repulsion", 0.2, 0, 1));
	gui.add(UseRepulsion.set("use_repulsion", true));
	shaderUniforms.add(maxSpeed.set("max_speed", 500, 0, 5000));
	shaderUniforms.add(attractorForce.set("attr_force", 2000, 0, 5000));
	shaderUniforms.add(ruleIterationMod.set("rule_iteration_mod", particleGroups, 0, particleGroups));
	shaderUniforms.add(only2D.set("only_2d", true));
	gui.add(shaderUniforms);
	gui.add(fps.set("fps", 60, 0, 60));

	gui.add(dirAsColor.set("Useless Button", true));
	//dirAsColor.addListener(this, &SwarmScene::dirAsColorChanged);

	InitSequences();
}

//--------------------------------------------------------------
void SwarmScene::InitSequences() {
	currentSequenceIndex = 100;
	nextSequenceTime = ofGetElapsedTimef() + 5;
	currentSequenceType = SequenceType::NormalAttraction;
	sequences.push_back(ParameterSequence(7, SequenceType::BlackHole, 10000));
	sequences.push_back(ParameterSequence(0.1f, SequenceType::Explosion));
	sequences.push_back(ParameterSequence(6, SequenceType::RepulsionStutter));
	sequences.push_back(ParameterSequence(5, SequenceType::NormalAttraction));
	sequences.push_back(ParameterSequence(20, SequenceType::BrainNeurons));
	sequences.push_back(ParameterSequence(15, SequenceType::Swarm));
	sequences.push_back(ParameterSequence(7, SequenceType::BlackHole));
	sequences.push_back(ParameterSequence(20, SequenceType::BrainNeurons));
	StartSequence();
}

//--------------------------------------------------------------
void SwarmScene::update() {
	fps = ofGetFrameRate();

	updateUserPositions();
	UpdateSequences();

	compute.begin();
	compute.setUniforms(shaderUniforms);
	compute.setUniform1i("use_attraction", (UseAttraction.get() ? 1 : 0));
	compute.setUniform1i("use_cohesion", (UseCohesion.get() ? 1 : 0));
	compute.setUniform1i("use_repulsion", (UseRepulsion.get() ? 1 : 0));
	compute.setUniform1f("timeLastFrame", ofGetLastFrameTime());
	compute.setUniform1f("elapsedTime", ofGetElapsedTimef());
	//compute.setUniform1i("particleAmount", particleAmount);
	compute.setUniform1i("particleAmount", ruleIterationMod * 1024);
	float size = 4;
	/*atractor3 = {ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.1),0,1,-ofGetWidth()*size,ofGetWidth()*size),
				ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.5),0,1,-ofGetHeight()*size,ofGetHeight()*size),
				ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.7),0,1,0,-ofGetHeight()*size)};*/
	atractor = { getProjectedPosition(mousePosition) };

	compute.setUniform3f("attractor", atractor.x, atractor.y, atractor.z);
	compute.setUniform2fv("hands", &user_positions[0].x, sizeof(ofVec2f) * 10);
	compute.setUniform2f("mouse", (float)ofGetMouseX(), (float)ofGetMouseY());

	compute.setUniform3f("baseColor", 0.0, 0.0, 0.0);

	// since each work group has a local_size of 1024 (this is defined in the shader)
	// we only have to issue 1 / 1024 workgroups to cover the full workload.
	// note how we add 1024 and subtract one, this is a fast way to do the equivalent
	// of std::ceil() in the float domain, i.e. to round up, so that we're also issueing
	// a work group should the total size of particles be < 1024
	compute.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	compute.end();

	particlesBuffer.copyTo(particlesBuffer2);
}

//--------------------------------------------------------------
void SwarmScene::draw() {
	ofClear(ofColor(0, 0, 0, 255));
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	camera.begin();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	particleShader.begin();

	particleShader.setUniform1f("alphaMod", 0.2);
	particleShader.setUniform1f("pointSize", 5);
	glPointSize(5);
	vbo.draw(GL_POINTS, 0, particles.size());
	particleShader.setUniform1f("alphaMod", 1.0);
	particleShader.setUniform1f("pointSize", 2);
	glPointSize(2);
	vbo.draw(GL_POINTS, 0, particles.size());

	particleShader.end();

	/*ofSetColor(ofColor(0, 0, 0, 255));
	ofNoFill();
	ofDrawBox(0,0,-ofGetHeight()*2,ofGetWidth()*4,ofGetHeight()*4,ofGetHeight()*4);*/
	camera.end();

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofFill();
	ofSetColor(ofColor::red);
	ofDrawRectangle(ofVec2f(user_positions[0].x, user_positions[0].y), 20, 20);
	ofDrawRectangle(mousePosition, 20, 20);
	ofSetColor(255);
}

//--------------------------------------------------------------
void SwarmScene::ColorSplash() {
	colorSplash.begin();
	colorSplash.setUniform3f("color", 1.0, 0.0, 0.0);
	ofVec3f projPos = getProjectedPosition(mousePosition);
	colorSplash.setUniform2f("mousePos", projPos.x, projPos.y);

	colorSplash.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	colorSplash.end();

	particlesBuffer.copyTo(particlesBuffer2);
}

//--------------------------------------------------------------
void SwarmScene::UserEnter() {
	userEnter.begin();
	userEnter.setUniform3f("color", 0.0, 1.0, 1.0);
	userEnter.setUniform2f("range", 0, 1024 * 2);
	userEnter.setUniform1f("user", 1);

	userEnter.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	userEnter.end();

	particlesBuffer.copyTo(particlesBuffer2);
}

//--------------------------------------------------------------
void SwarmScene::dirAsColorChanged(bool& dirAsColor) {
	if (dirAsColor) {
		vbo.enableColors();
	}
	else {
		vbo.disableColors();
	}
}

//--------------------------------------------------------------
void SwarmScene::UpdateMousePos(int x, int y, string action) {
	if (isInBounds(x, y))
	{
		mousePosition.set((ofVec3f(x, y, 0.0)));
		if (action == "click") {
			mouseClickPosition.set((ofVec3f(x, y, 0.0)));
		}
	}
}

//--------------------------------------------------------------
void SwarmScene::keyPressed(int key) {
}

//--------------------------------------------------------------
void SwarmScene::keyReleased(int key) {
	if (key == 'f') {
		ofToggleFullscreen();
	}

	if (key == 't') {
		ColorSplash();
	}

	if (key == 'r') {
		UserEnter();
	}
}

//--------------------------------------------------------------
void SwarmScene::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void SwarmScene::mouseDragged(int x, int y, int button) {
	UpdateMousePos(x, y);
}

//--------------------------------------------------------------
void SwarmScene::mousePressed(int x, int y, int button) {

	isPressingMouse = true;
	UpdateMousePos(x, y, "click");

	switch (button) {
	case 0:

		break;
	case 2:

		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void SwarmScene::mouseReleased(int x, int y, int button) {
	isPressingMouse = false;
	UpdateMousePos(x, y);
}

//--------------------------------------------------------------
void SwarmScene::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void SwarmScene::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void SwarmScene::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void SwarmScene::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void SwarmScene::dragEvent(ofDragInfo dragInfo) {

}


//--------------------------------------------------------------
void SwarmScene::UpdateSequences() {
	if (ofGetElapsedTimef() > nextSequenceTime) {
		currentSequenceIndex++;
		if (currentSequenceIndex >= sequences.size() || currentSequenceIndex < 0) currentSequenceIndex = 0;

		currentSequenceType = sequences[currentSequenceIndex].sequence;
		currentSequenceMod = sequences[currentSequenceIndex].modifier;
		StartSequence();
		lastSequenceTime = ofGetElapsedTimef();
		nextSequenceTime = lastSequenceTime + sequences[currentSequenceIndex].duration;
	}

	UpdateParameters();
}

//--------------------------------------------------------------
void SwarmScene::StartSequence() {
	ActivateRules();

	switch (currentSequenceType)
	{
	case SequenceType::BlackHole:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(1250 * currentSequenceMod);
		repulsionCoeff.set(repulsionCoeff.getMin());
		maxSpeed.set(2500 * currentSequenceMod);
		break;
	case SequenceType::Explosion:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(attractorForce.getMin());
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(maxSpeed.getMax());
		break;
	case SequenceType::NormalAttraction:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(2500);
		repulsionCoeff.set(0.1f);
		maxSpeed.set(500);
		break;
	case SequenceType::BrainNeurons:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(5000);
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(5000);
		break;
	case SequenceType::Swarm:
		attractionCoeff.set(attractionCoeff.getMax());
		attractorForce.set(5000);
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(5000);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void SwarmScene::ActivateRules() {
	UseAttraction.set(true);
	UseCohesion.set(true);
	UseRepulsion.set(true);
}

//--------------------------------------------------------------
void SwarmScene::UpdateParameters() {
	float currentTime = ofGetElapsedTimef();

	float mod = ruleIterationMod.getMax() / 2 + (abs(sin(ofGetElapsedTimef() / 10)) * ruleIterationMod.getMax() / 2);
	ruleIterationMod.set(mod);

	switch (currentSequenceType)
	{
	case SequenceType::BlackHole: {

		break;
	}
	case SequenceType::Explosion: {

		break;
	}
	case SequenceType::NormalAttraction: {

		break;
	}
	case SequenceType::BrainNeurons:
	{
		float diff = currentTime - lastSequenceTime;
		float newSpeed = maxSpeed.getMax() - 250 * diff;
		newSpeed = std::max(newSpeed, 500.0f);
		maxSpeed.set(newSpeed);
		break;
	}
	case SequenceType::Swarm:
	{
		float diff = currentTime - lastSequenceTime;
		float newSpeed = maxSpeed.getMax() - 400 * diff;
		newSpeed = std::max(newSpeed, 750.0f);
		maxSpeed.set(newSpeed);
		break;
	}
	case SequenceType::RepulsionStutter:
	{
		float diff = currentTime - (int) currentTime;
		if(diff < 0.1f) UseRepulsion.set(true);
		else UseRepulsion.set(false);
		break;
	}
	default:
		break;
	}
}