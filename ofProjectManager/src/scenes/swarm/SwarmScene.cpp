#include "SwarmScene.h"

#include "ofConstants.h"

SwarmScene::SwarmScene() : ccScene("Swarm") {

}

//--------------------------------------------------------------
void SwarmScene::setup() {
	camera.setFarClip(ofGetWidth() * 100);
	ofShowCursor();
	particleGroups = 40;
	maxParticleDepth = ORIGINAL_MAX_PARTICLE_DEPTH;
	minParticleDepth = ORIGINAL_MIN_PARTICLE_DEPTH;
	particleAmount = 1024 * particleGroups;



	particles.resize(std::max(particleAmount, 1024));
	int i = 0;
	float step = (maxParticleDepth / (float)particleAmount);
	for (auto& p : particles) {
		p.pos.x = ofRandom(0, 1000);
		p.pos.y = ofRandom(0, 1000);
		p.pos.z = ofRandom(-maxParticleDepth, -minParticleDepth);
		p.pos.w = 1;

		p.u = { 0,0,0,0 };
		p.color = { 1, 1, 1, 1 };
		p.initialPos = p.pos;
		p.bufferPos = p.pos;

		//int .z = (int)(-step * i);
		p.unique = { 0, i ,0, 0 };

		i++;
	}

	filesystem::path shader_path = getShaderPath();

	introShader.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "intro.comp");
	introShader.linkProgram();

	behaviorShader.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "particleBehavior.comp");
	behaviorShader.linkProgram();

	interactionShader.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "userInteraction.comp");
	interactionShader.linkProgram();

	userEnter.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "swarmUserEnter.comp");
	userEnter.linkProgram();

	changeDepthShader.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "changeDepth.comp");
	changeDepthShader.linkProgram();

	particleShader.load(shader_path / "swarm.vert", shader_path / "swarm.frag");
	userCircleShader.load(shader_path / "userCircles.vert", shader_path / "userCircles.frag");

	vector<Particle> sorted = SortParticles();

	particlesBuffer.allocate(particles, GL_DYNAMIC_DRAW);
	particlesBuffer2.allocate(particles, GL_DYNAMIC_DRAW);
	particlesBuffer3.allocate(sorted, GL_DYNAMIC_DRAW);

	particlesBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
	particlesBuffer2.bindBase(GL_SHADER_STORAGE_BUFFER, 1);
	particlesBuffer3.bindBase(GL_SHADER_STORAGE_BUFFER, 3);

	vbo.setVertexBuffer(particlesBuffer3, 4, sizeof(Particle));
	vbo.setColorBuffer(particlesBuffer3, sizeof(Particle), sizeof(glm::vec4) * 2);

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
	shaderUniforms.add(particleColorStart.set("particle_color_start", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	shaderUniforms.add(particleColorEnd.set("particle_color_end", glm::vec3(0.9, 0.9, 0.9), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
	shaderUniforms.add(freezeParticles.set("freeze_particles", false));
	gui.add(shaderUniforms);
	gui.add(isSequencerInControl.set("Let Sequencer control", true));
	gui.add(fps.set("fps", 60, 0, 60)); 

	user_circle_alpha = 1.0f;
	user_circle_radius = 100;

	gui.add(dirAsColor.set("Useless Button", true));
	//dirAsColor.addListener(this, &SwarmScene::dirAsColorChanged);

	InitSequences();
}

//--------------------------------------------------------------
vector<SwarmScene::Particle> SwarmScene::SortParticles() {
	vector<Particle> output(particles);
	std::sort(output.begin(), output.end(), SortByDepthOperator());

	vector<int> lookup(output.size());

	int index = 0;
	for (auto const& p : output) {
		lookup[p.unique.y] = index;
		index++;
	}

	index = 0;
	for (auto& p : particles) {
		p.unique.y = lookup[index];
		index++;
	}

	/*for (auto& p : particles) {
		cout << "Pos: " << p.pos << " refers id: " << p.unique.y << " is pos: " << output[p.unique.y].pos << endl;
	};*/

	return output;
}

//--------------------------------------------------------------
void SwarmScene::update() {
	fps = ofGetFrameRate();

	UpdateSequence();
	ApplyParticleRules();
	ApplyInteraction();
}

// ####################
// CodeSection: Drawing
// ####################

void SwarmScene::draw() {
	ofClear(ofColor(0, 0, 0, 255));
	ofPushStyle();
	camera.begin();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	ofEnableDepthTest();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	DrawParticles();

	camera.end();

	DrawUserCircles();

	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255);
	ofPopStyle();
}

void SwarmScene::DrawParticles() {
	particleShader.begin();

	// Inner particle core
	particleShader.setUniform1i("max_particle_depth", maxParticleDepth);
	particleShader.setUniform1f("alphaMod", 1.0);
	particleShader.setUniform1f("pointSize", 2);
	glPointSize(2);
	vbo.draw(GL_POINTS, 0, particles.size());

	ofDisableDepthTest();

	// Outer more transparent particle ring
	particleShader.setUniform1f("alphaMod", 0.3);
	particleShader.setUniform1f("pointSize", 5);
	glPointSize(5);
	vbo.draw(GL_POINTS, 0, particles.size());

	particleShader.end();
}

void SwarmScene::DrawUserCircles() {
	ccUserManager& um = ccUserManager::get();
	array<ofVec3f, MAX_SWARM_HANDS> user_hands = GetUserHandsArray(ccScene::CoordinateSystem::Screen);
	int hand_count = min(MAX_SWARM_HANDS, um.getUserCount() * 2);

	for (int i = 0; i < hand_count; i++)
	{
		userCircleShader.begin();

		ofFill();
		ofSetColor(ofColor::cyan);


		userCircleShader.setUniform2f("screen_resolution", ofVec2f(ofGetWidth(), ofGetHeight()));
		userCircleShader.setUniform3f("hand_position", user_hands[i]); 
		userCircleShader.setUniform1f("radius", user_circle_radius);
		userCircleShader.setUniform1f("user_circle_alpha", user_circle_alpha);

		ofDrawCircle(user_hands[i], user_circle_radius);

		//ofDisableDepthTest();

		userCircleShader.end();
	}

	//cout << "rez: " << ofVec2f(ofGetWidth(), ofGetHeight()) << " - hand pos 0 : " << user_hands[0] << endl;

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
	if (key == 't') {
		cout << "Retrigger Scene Intro" << endl;
		SetSequence(ParameterSequence(3, SequenceName::Intro));
	}
	else if (key == 'd') {
		ChangeParticleDepth(5000, 10000);
	}
	else if (key == 'r') {
		RevertParticleDepthToOriginal();
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

// ######################
// CodeSection: Sequencer
// ######################

// Creates an arrray with sequences
void SwarmScene::InitSequences() {
	currentSequenceIndex = 0;
	sequences.push_back(ParameterSequence(20, SequenceName::BrainNeuronsFlashLight));
	sequences.push_back(ParameterSequence(20, SequenceName::CrazyTestOne));
	sequences.push_back(ParameterSequence(20, SequenceName::BrainNeuronsDense));
	sequences.push_back(ParameterSequence(20, SequenceName::Swarm));
	sequences.push_back(ParameterSequence(20, SequenceName::BrainNeuronsCoarse));
	sequences.push_back(ParameterSequence(20, SequenceName::CrazyClose));
	sequences.push_back(ParameterSequence(20, SequenceName::RepulsionStutter));
	sequences.push_back(ParameterSequence(20, SequenceName::BrainNeurons));
	sequences.push_back(ParameterSequence(20, SequenceName::VeryDense));
	sequences.push_back(ParameterSequence(20, SequenceName::VeryClose));

	//	sequences.push_back(ParameterSequence(20, SequenceName::BrainNeurons));
	//	sequences.push_back(ParameterSequence(7, SequenceName::BlackHole, 5));
	//	sequences.push_back(ParameterSequence(4, SequenceName::Explosion));
	//	sequences.push_back(ParameterSequence(0.1f, SequenceName::NormalAttraction));
	//	sequences.push_back(ParameterSequence(7, SequenceName::BlackHole, 3));
	SetSequence(sequences[currentSequenceIndex]);
}

// Sets currentSequence to a new sequence and immediately activates it.
void SwarmScene::SetSequence(ParameterSequence sequence) {
	currentSequence = sequence;
	StartSequence();
}

// Activates the current sequence by setting the intial parameters
void SwarmScene::StartSequence() {
	lastSequenceTime = ofGetElapsedTimef();
	nextSequenceTime = lastSequenceTime + currentSequence.duration;

	RevertParticleDepthToOriginal();
	particleColorStart.set(ofVec3f(0, 0, 0));
	particleColorEnd.set(ofVec3f(0.9f, 0.9f, 0.9f));
	user_circle_alpha = 1.0f;
	user_circle_radius = 100;

	ActivateRules();

	switch (currentSequence.sequenceType)
	{
	case SequenceName::BlackHole:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(1250 * currentSequence.modifier);
		repulsionCoeff.set(repulsionCoeff.getMin());
		maxSpeed.set(2500 * currentSequence.modifier);
		ChangeParticleDepth(3000, ORIGINAL_MAX_PARTICLE_DEPTH);
		break;
	case SequenceName::RepulsionStutter:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(attractorForce.getMax());
		repulsionCoeff.set(repulsionCoeff.getMin());
		maxSpeed.set(4000);
		ChangeParticleDepth(3000, ORIGINAL_MAX_PARTICLE_DEPTH);
		break;
	case SequenceName::Explosion:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(attractorForce.getMin());
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(maxSpeed.getMax());
		break;
	case SequenceName::CrazyTestOne:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(attractorForce.getMax());
		repulsionCoeff.set(0.3f);
		maxSpeed.set(2500);
		break;
	case SequenceName::NormalAttraction:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(3500);
		repulsionCoeff.set(0.1f);
		maxSpeed.set(500);
		break;
	case SequenceName::BrainNeurons:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(5000);
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(5000);
		break;
	case SequenceName::BrainNeuronsCoarse:
	case SequenceName::BrainNeuronsFlashLight:
	{
		attractionCoeff.set(attractionCoeff.getMin());
		repulsionCoeff.set(repulsionCoeff.getMax());
		attractorForce.set(1500);
		maxSpeed.set(2500);
		break;
	}
	case SequenceName::BrainNeuronsDense:
	{
		attractionCoeff.set(attractionCoeff.getMin());
		repulsionCoeff.set(0.15f);
		attractorForce.set(attractorForce.getMax());
		maxSpeed.set(2500);
		ChangeParticleDepth(3000, 6000);
		break;
	}
	case SequenceName::Swarm:
		attractionCoeff.set(0.2f);
		attractorForce.set(5000);
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(5000);
		break;
	case SequenceName::VeryClose:
		attractionCoeff.set(0);
		attractorForce.set(5000);
		repulsionCoeff.set(0.5f);
		maxSpeed.set(5000);
		ChangeParticleDepth(1000, 3000);
		break;
	case SequenceName::VeryDense:
		attractionCoeff.set(0);
		attractorForce.set(5000);
		repulsionCoeff.set(0.5f);
		maxSpeed.set(5000);
		ChangeParticleDepth(1500, 1750);
		break;
	case SequenceName::CrazyClose:
		attractionCoeff.set(0);
		attractorForce.set(5000);
		repulsionCoeff.set(0.5f);
		maxSpeed.set(5000);
		ChangeParticleDepth(500, 2500);
		break;
	case SequenceName::Intro:
	{
		particleColorStart.set(particleColorStart.getMin());
		particleColorEnd.set(particleColorEnd.getMin());

		repulsionCoeff.set(0.75f);
		maxSpeed.set(2500);
		attractorForce.set(3000);

		// This ensures that when this sequence is finished, the sequence loop starts with the sequence that was running before that
		//currentSequenceIndex -= 1;
		//if (currentSequenceIndex < 0) currentSequenceIndex = 0;
		break;
	}
	default:
		break;
	}

	UpdateSequence();
}

// Activates all swarm movement rules
void SwarmScene::ActivateRules() {
	UseAttraction.set(true);
	UseCohesion.set(true);
	UseRepulsion.set(true);
}

// If nextSequenceTime has arrived, activate the next sequence in the sequences vector
void SwarmScene::CheckForNextSequence() {
	if (ofGetElapsedTimef() > nextSequenceTime) {
		currentSequenceIndex++;
		if (currentSequenceIndex >= sequences.size() || currentSequenceIndex < 0) currentSequenceIndex = 0;

		SetSequence(sequences[currentSequenceIndex]);
		cout << "Swarm-Scene switched to next sequence with index " << currentSequenceIndex << " . Will wait for " << sequences[currentSequenceIndex].duration << " seconds now." << endl;
	}
}

// Change the parameters over time according to the current scene
void SwarmScene::UpdateSequence() {
	if (isSequencerInControl.get() == false) return;

	CheckForNextSequence();

	float currentTime = ofGetElapsedTimef();
	float timeSinceStart = currentTime - lastSequenceTime;
	// ofGetElapsedTime is sometimes buggy and results in a negative timeSinceStart, so this line prevents anything funky from happening
	timeSinceStart = max(0.f, timeSinceStart);

	bool fadeOutColor = false;
	if (currentSequence.duration - timeSinceStart < 3.f && currentSequence.sequenceType != SequenceName::Intro && currentSequence.sequenceType != SequenceName::Outro) fadeOutColor = true;

	//float mod = ruleIterationMod.getMax() / 2 + (abs(sin(ofGetElapsedTimef() / 10)) * ruleIterationMod.getMax() / 2);
	//ruleIterationMod.set(mod);

	float percentage = min(timeSinceStart / currentSequence.duration, 1.f);

	switch (currentSequence.sequenceType)
	{
	case SequenceName::BlackHole: {
	}
	case SequenceName::Explosion: {

		break;
	}
	case SequenceName::CrazyTestOne: {
		float newMaxDepth = 5000;
		float newMinDepth = abs(sin(ofGetElapsedTimef())) * newMaxDepth - 100;
		ChangeParticleDepth(newMinDepth, newMaxDepth);

		if(!fadeOutColor) particleColorStart.set(ofVec3f(min(0.8f, timeSinceStart * 0.5f), 0, 0));

		break;
	}
	case SequenceName::NormalAttraction: {

		break;
	}
	case SequenceName::BrainNeurons:
	{
		float newSpeed = maxSpeed.getMax() - 200 * timeSinceStart;
		newSpeed = std::max(newSpeed, 1500.0f);
		maxSpeed.set(newSpeed);

		if (!fadeOutColor) particleColorStart.set(ofVec3f(min(0.15f, timeSinceStart * 0.5f), 0, 0));
		break;
	}
	case SequenceName::BrainNeuronsDense:
	{
		if (!fadeOutColor) particleColorStart.set(ofVec3f(min(0.3f, timeSinceStart * 0.5f), 0, 0));
		break;
	}
	case SequenceName::BrainNeuronsCoarse:
	{
		float newSpeed = min(maxSpeed.getMax(), maxSpeed.get() + 500 * timeSinceStart);
		maxSpeed.set(newSpeed);
		float newForce = sin(ofGetElapsedTimef() / 2) * 500 + 1000;
		attractorForce.set(newForce);

		break;
	}	
	case SequenceName::BrainNeuronsFlashLight:
	{
		float newSpeed = min(maxSpeed.getMax(), maxSpeed.get() + 500 * timeSinceStart);
		maxSpeed.set(newSpeed);
		float newForce = sin(ofGetElapsedTimef() / 2) * 500 + 1000;
		attractorForce.set(newForce); 
		user_circle_radius = min(400.f, 100 + timeSinceStart * 100 * 1.f);


		float newWhite = pow(abs(sin(ofGetElapsedTimef())), 5) * 0.5f;
		if (timeSinceStart < 2.f) newWhite = (1 - (timeSinceStart / 2.0f)) * 0.9f;
		if (!fadeOutColor) particleColorEnd.set(ofVec3f(newWhite));
		break;
	}
	case SequenceName::Swarm:
	{
		float newSpeed = maxSpeed.getMax() - 400 * timeSinceStart;
		newSpeed = std::max(newSpeed, 750.0f);
		maxSpeed.set(newSpeed);


		float newRed = pow(abs(sin(ofGetElapsedTimef())), 25) * 0.3f;
		if (!fadeOutColor) particleColorStart.set(ofVec3f(newRed, 0, 0));
		break;
	}
	case SequenceName::RepulsionStutter:
	{
		float newRepulsion = (pow(abs(sin(ofGetElapsedTimef()* 1.5f)),20)) * 0.75f;
		repulsionCoeff.set(newRepulsion);
		break;
	}
	case SequenceName::Intro:
	{
		particleColorStart.set(glm::vec3(0, 0, 0) * percentage);
		particleColorEnd.set(glm::vec3(0.9, 0.9, 0.9) * percentage);
		user_circle_alpha = min(1.f, percentage);
		break;
	}
	case SequenceName::Outro:
	{
		float percentageInverted = 1 - percentage;
		ofVec3f increment = ofVec3f(-1.f) * ofGetLastFrameTime() * 0.35f;

		particleColorStart.set(particleColorStart.get() + increment);
		particleColorEnd.set(particleColorEnd.get() + increment);

		user_circle_alpha = max(0.f, percentageInverted);

		break;
	}
	case SequenceName::VeryDense:
	{
		float section = fmod(timeSinceStart, 9);
		if (section < 5) {
			float newRepulsion = section / 5 * 0.5f;
			repulsionCoeff.set(newRepulsion);
		}
		else {
			repulsionCoeff.set(0);
		}
		break;
	}
	case SequenceName::CrazyClose:
	{
		float newMinDepth = 500; 
		float newMaxDepth = abs(sin(ofGetElapsedTimef())) * 2000 + 600;
		ChangeParticleDepth(newMinDepth, newMaxDepth);

		float newMaxSpeed = pow(abs(sin(ofGetElapsedTimef()*2)), 7) * 3000 + 2000;
		maxSpeed.set(newMaxSpeed);
		break;
	}
	default:
		break;
	}

	// Put Values back to original smoothly
	if (fadeOutColor) {
		ofVec3f startCol = particleColorStart.get();
		startCol.x = max(0.0, startCol.x - (0.1f + pow(startCol.x, 2) * 0.75f) * ofGetLastFrameTime());
		particleColorStart.set(startCol);

		ofVec3f endCol = particleColorEnd.get();
		float newWhite = min(0.9, endCol.x + (0.1f + pow(endCol.x, 2) * 0.75f) * ofGetLastFrameTime());
		particleColorEnd.set(ofVec3f(newWhite));

		user_circle_radius = max(100.0, user_circle_radius - (0.1f + pow(user_circle_radius, 2) * 0.75f) * ofGetLastFrameTime());
	}

	//cout << "dur: " << currentSequence.duration << " time since start: " << timeSinceStart << endl;
}

// ####################
// CodeSection: Shaders
// ####################

void SwarmScene::ApplyParticleRules() {
	behaviorShader.begin();

	behaviorShader.setUniforms(shaderUniforms);
	behaviorShader.setUniform1f("timeLastFrame", ofGetLastFrameTime());
	behaviorShader.setUniform1f("elapsedTime", ofGetElapsedTimef());
	behaviorShader.setUniform1i("use_attraction", (UseAttraction.get() ? 1 : 0));
	behaviorShader.setUniform1i("use_cohesion", (UseCohesion.get() ? 1 : 0));
	behaviorShader.setUniform1i("use_repulsion", (UseRepulsion.get() ? 1 : 0));
	behaviorShader.setUniform1i("freeze_particles", (freezeParticles.get() ? 1 : 0));
	//behaviorShader.setUniform1i("particleAmount", particleAmount);
	behaviorShader.setUniform1i("particleAmount", ruleIterationMod * 1024);
	behaviorShader.setUniform1i("min_particle_depth", minParticleDepth);
	behaviorShader.setUniform1i("max_particle_depth", maxParticleDepth);

	behaviorShader.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	behaviorShader.end();

	particlesBuffer.copyTo(particlesBuffer2);
}

void SwarmScene::ApplyInteraction() {
	interactionShader.begin();
	interactionShader.setUniforms(shaderUniforms);
	interactionShader.setUniform1i("freeze_particles", (freezeParticles.get() ? 1 : 0));
	interactionShader.setUniform1f("timeLastFrame", ofGetLastFrameTime());
	interactionShader.setUniform1f("elapsedTime", ofGetElapsedTimef());
	interactionShader.setUniform1i("min_particle_depth", minParticleDepth);
	interactionShader.setUniform1i("max_particle_depth", maxParticleDepth);

	ccUserManager& um = ccUserManager::get();
	array<ofVec3f, MAX_SWARM_HANDS> user_hands = GetUserHandsArray(ccScene::CoordinateSystem::World);
	int hand_count = min(MAX_SWARM_HANDS, um.getUserCount() * 2);
	if (user_hands.empty()) {
		interactionShader.setUniform1i("hand_count", 0);
	}
	else {
		interactionShader.setUniform3fv("hands", &user_hands[0].x, sizeof(ofVec3f) * 10);
		interactionShader.setUniform1i("hand_count", hand_count);
	}

	//cout << "0. Hand Position: " << user_hands[0] << " hand count: " << hand_count << endl;
	/*cout << "MousePosition: " << getProjectedPosition(mousePosition) << endl;
	cout << "Handcount: " << user_hands.size() << endl;*/

	interactionShader.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	interactionShader.end();

	particlesBuffer.copyTo(particlesBuffer2);
}

void SwarmScene::ChangeParticleDepth(float newMin, float newMax) {
	changeDepthShader.begin();
	changeDepthShader.setUniform1i("original_min_particle_depth", ORIGINAL_MIN_PARTICLE_DEPTH);
	changeDepthShader.setUniform1i("original_max_particle_depth", ORIGINAL_MAX_PARTICLE_DEPTH);
	changeDepthShader.setUniform1i("new_min_particle_depth", newMin);
	changeDepthShader.setUniform1i("new_max_particle_depth", newMax);

	changeDepthShader.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	changeDepthShader.end();

	particlesBuffer.copyTo(particlesBuffer2);

	minParticleDepth = newMin;
	maxParticleDepth = newMax;
}

void SwarmScene::RevertParticleDepthToOriginal() {
	changeDepthShader.begin();
	changeDepthShader.setUniform1i("original_min_particle_depth", ORIGINAL_MIN_PARTICLE_DEPTH);
	changeDepthShader.setUniform1i("original_max_particle_depth", ORIGINAL_MAX_PARTICLE_DEPTH);
	changeDepthShader.setUniform1i("new_min_particle_depth", ORIGINAL_MIN_PARTICLE_DEPTH);
	changeDepthShader.setUniform1i("new_max_particle_depth", ORIGINAL_MAX_PARTICLE_DEPTH);

	changeDepthShader.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	changeDepthShader.end();

	particlesBuffer.copyTo(particlesBuffer2);

	minParticleDepth = ORIGINAL_MIN_PARTICLE_DEPTH;
	maxParticleDepth = ORIGINAL_MAX_PARTICLE_DEPTH;
}


array<ofVec3f, MAX_SWARM_HANDS> SwarmScene::GetUserHandsArray(ccScene::CoordinateSystem system) {
	vector<ofVec3f> user_hands = getHandPositions(system);
	array<ofVec3f, MAX_SWARM_HANDS> hands_max;

	if (user_hands.empty()) return hands_max;

	hands_max.fill(ofVec3f(0.f));

	int n = user_hands.size();
	int handCounter = 0;
	for (size_t i = 0; i < n; i++) {
		if (i >= hands_max.max_size()) break;
		if ((!glm::isnan(user_hands[i].x)) && (!glm::isnan( user_hands[i].y )) && (!glm::isnan( user_hands[i].z ))) {
			hands_max[handCounter] = user_hands[i];
			handCounter++;
		} 
	}

	return hands_max;
}

// ##############################
// CodeSection: Scene Transitions
// ##############################

// Triggered when this scene is opened
float SwarmScene::SceneIntro() {
	cout << "Swarm Intro Triggered" << endl;

	introShader.begin();

	introShader.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	introShader.end();

	particlesBuffer.copyTo(particlesBuffer2);

	SetSequence(ParameterSequence(4, SequenceName::Intro));

	return 5.0f;
}

// Triggered when this scene is closed
float SwarmScene::SceneOutro() {
	cout << "Swarm Outro Triggered" << endl;

	SetSequence(ParameterSequence(3, SequenceName::Outro));

	return 5.0f;
}