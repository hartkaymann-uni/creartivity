#include "SwarmScene.h"

#include "ofConstants.h"

SwarmScene::SwarmScene() : ccScene("Swarm") {

}

//--------------------------------------------------------------
void SwarmScene::setup() {
	camera.setFarClip(ofGetWidth() * 100);
	ofShowCursor();
	particleGroups = 400;
	maxParticleDepth = 10000;
	particleAmount = 1024 * particleGroups;



	particles.resize(std::max(particleAmount, 1024));
	int i = 0;
	float step = (maxParticleDepth / (float)particleAmount);
	for (auto& p : particles) {
		p.pos.x = ofRandom(0, 1000);
		p.pos.y = ofRandom(0, 1000);
		p.pos.z = ofRandom(-maxParticleDepth, 0);
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
	compute.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "swarm.comp");
	compute.linkProgram();

	//colorSplash.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "colorSplash.comp");
	//colorSplash.linkProgram();

	introShader.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "intro.comp");
	introShader.linkProgram();

	introShader.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "intro.comp");
	introShader.linkProgram();

	userEnter.setupShaderFromFile(GL_COMPUTE_SHADER, shader_path / "swarmUserEnter.comp");
	userEnter.linkProgram();

	particleShader.load(shader_path / "swarm.vert", shader_path / "swarm.frag");

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
	gui.add(fps.set("fps", 60, 0, 60));

	gui.add(dirAsColor.set("Useless Button", true));
	//dirAsColor.addListener(this, &SwarmScene::dirAsColorChanged);

	InitSequences();

	DoUpdateSequence = true;
}

//--------------------------------------------------------------
void SwarmScene::InitSequences() {
	currentSequenceIndex = 100;
	nextSequenceTime = ofGetElapsedTimef() + 2;
	SetSequence(ParameterSequence(1, SequenceName::NormalAttraction));
	sequences.push_back(ParameterSequence(16, SequenceName::BrainNeurons));
	sequences.push_back(ParameterSequence(7, SequenceName::BlackHole, 5));
	sequences.push_back(ParameterSequence(4, SequenceName::Explosion));
	sequences.push_back(ParameterSequence(0.1f, SequenceName::NormalAttraction));
	sequences.push_back(ParameterSequence(16, SequenceName::BrainNeurons));
	sequences.push_back(ParameterSequence(10, SequenceName::Swarm));
	sequences.push_back(ParameterSequence(5, SequenceName::RepulsionStutter));
	sequences.push_back(ParameterSequence(7, SequenceName::BlackHole, 3));
}

//--------------------------------------------------------------
vector<SwarmScene::Particle> SwarmScene::SortParticles() {
	vector<Particle> output(particles);
	std::sort(output.begin(), output.end(), SortByDepthOperator());

	/*for (auto &p : particles) {
		int index = 0;
		for (auto const &x : output) {
			if (p.unique.y == x.unique.y) {
				p.unique.y = index;
				break;
			}
			index++;
		}
	}*/

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

	updateUserPositions();
	UpdateSequences();

	compute.begin();
	compute.setUniforms(shaderUniforms);
	compute.setUniform1i("use_attraction", (UseAttraction.get() ? 1 : 0));
	compute.setUniform1i("use_cohesion", (UseCohesion.get() ? 1 : 0));
	compute.setUniform1i("use_repulsion", (UseRepulsion.get() ? 1 : 0));
	compute.setUniform1i("freeze_particles", (freezeParticles.get() ? 1 : 0));
	compute.setUniform1f("timeLastFrame", ofGetLastFrameTime());
	compute.setUniform1f("elapsedTime", ofGetElapsedTimef());
	//compute.setUniform1i("particleAmount", particleAmount);
	compute.setUniform1i("particleAmount", ruleIterationMod * 1024);
	compute.setUniform1i("max_particle_depth", maxParticleDepth);
	float size = 4;
	/*atractor3 = {ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.1),0,1,-ofGetWidth()*size,ofGetWidth()*size),
				ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.5),0,1,-ofGetHeight()*size,ofGetHeight()*size),
				ofMap(ofNoise(ofGetElapsedTimef()*0.9+0.7),0,1,0,-ofGetHeight()*size)};*/
	atractor = { getProjectedPosition(mousePosition) };

	compute.setUniform3f("attractor", atractor.x, atractor.y, atractor.z);

	/*std::array<T, N> arr;
	std::copy_n(vec.begin(), N, arr.begin());*/

	vector<ccUser> hello = userManager->getUserVec();

	array<ofVec2f, 10> user_positions;
	compute.setUniform2fv("hands", &user_positions[0].x, sizeof(ofVec2f) * 10);
	compute.setUniform2fv("hands", &user_positions[0].x, sizeof(ofVec2f) * 10);
	compute.setUniform2f("mouse", (float)ofGetMouseX(), (float)ofGetMouseY());

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

	camera.begin();
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);*/
	ofEnableDepthTest();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	particleShader.begin();

	particleShader.setUniform1i("max_particle_depth", maxParticleDepth);
	particleShader.setUniform1f("alphaMod", 1.0);
	particleShader.setUniform1f("pointSize", 2);
	glPointSize(2);
	vbo.draw(GL_POINTS, 0, particles.size());

	ofDisableDepthTest();

	particleShader.setUniform1f("alphaMod", 0.3);
	particleShader.setUniform1f("pointSize", 5);
	glPointSize(5);
	vbo.draw(GL_POINTS, 0, particles.size());

	particleShader.end();

	camera.end();

	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofFill();
	//ofSetColor(ofColor::red);
	//ofDrawRectangle(ofVec2f(user_positions[0].x, user_positions[0].y), 20, 20);
	//ofDrawRectangle(mousePosition, 20, 20);
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
		cout << "Retrigger Scene Intro" << endl;
		SetSequence(ParameterSequence(3, SequenceName::Intro));
	}

	if (key == 's') {
		DoUpdateSequence = !DoUpdateSequence;
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
	if (DoUpdateSequence == false) return;
	if (ofGetElapsedTimef() > nextSequenceTime) {
		currentSequenceIndex++;
		if (currentSequenceIndex >= sequences.size() || currentSequenceIndex < 0) currentSequenceIndex = 0;

		SetSequence(sequences[currentSequenceIndex]);
	}

	UpdateParameters();
}

//--------------------------------------------------------------
void SwarmScene::SetSequence(ParameterSequence sequence) {
	currentSequence = sequence;

	StartSequence();
	lastSequenceTime = ofGetElapsedTimef();
	nextSequenceTime = lastSequenceTime + currentSequence.duration;

	UpdateParameters();
}

//--------------------------------------------------------------
void SwarmScene::StartSequence() {
	ActivateRules();

	switch (currentSequence.sequenceType)
	{
	case SequenceName::BlackHole:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(1250 * currentSequence.modifier);
		repulsionCoeff.set(repulsionCoeff.getMin());
		maxSpeed.set(2500 * currentSequence.modifier);
		break;
	case SequenceName::Explosion:
		attractionCoeff.set(attractionCoeff.getMin());
		attractorForce.set(attractorForce.getMin());
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(maxSpeed.getMax());
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
	case SequenceName::Swarm:
		attractionCoeff.set(attractionCoeff.getMax());
		attractorForce.set(5000);
		repulsionCoeff.set(repulsionCoeff.getMax());
		maxSpeed.set(5000);
		break;
	case SequenceName::Intro:
	{
		particleColorStart.set(particleColorStart.getMin());
		particleColorEnd.set(particleColorEnd.getMin());
	}
	default:
		break;
	}
}

//--------------------------------------------------------------
void SwarmScene::ActivateRules() {
	//UseAttraction.set(true);
	//UseCohesion.set(true);
	UseRepulsion.set(true);
}

//--------------------------------------------------------------
void SwarmScene::UpdateParameters() {
	float currentTime = ofGetElapsedTimef();

	float mod = ruleIterationMod.getMax() / 2 + (abs(sin(ofGetElapsedTimef() / 10)) * ruleIterationMod.getMax() / 2);
	ruleIterationMod.set(mod);

	switch (currentSequence.sequenceType)
	{
	case SequenceName::BlackHole: {

		break;
	}
	case SequenceName::Explosion: {

		break;
	}
	case SequenceName::NormalAttraction: {

		break;
	}
	case SequenceName::BrainNeurons:
	{
		float diff = currentTime - lastSequenceTime;
		float newSpeed = maxSpeed.getMax() - 200 * diff;
		newSpeed = std::max(newSpeed, 1500.0f);
		maxSpeed.set(newSpeed);
		break;
	}
	case SequenceName::Swarm:
	{
		float diff = currentTime - lastSequenceTime;
		float newSpeed = maxSpeed.getMax() - 400 * diff;
		newSpeed = std::max(newSpeed, 750.0f);
		maxSpeed.set(newSpeed);
		break;
	}
	case SequenceName::RepulsionStutter:
	{
		float diff = currentTime - (int)currentTime;
		if (diff < 0.1f) UseRepulsion.set(true);
		else UseRepulsion.set(false);
		break;
	}
	case SequenceName::Intro:
	{
		float diff = currentTime - lastSequenceTime;

		float percentage = min(diff / currentSequence.duration, 1.f);

		particleColorStart.set(glm::vec3(0, 0, 0) * percentage);
		particleColorEnd.set(glm::vec3(0.9, 0.9, 0.9) * percentage);
		break;
	}
	default:
		break;
	}
}

//--------------------------------------------------------------
float SwarmScene::SceneIntro() {
	cout << "Swarm Intro Triggered" << endl;

	introShader.begin();

	introShader.dispatchCompute((particles.size() + 1024 - 1) / 1024, 1, 1);

	introShader.end();

	particlesBuffer.copyTo(particlesBuffer2);

	SetSequence(ParameterSequence(4, SequenceName::Intro));

	return 4.f;
}

//--------------------------------------------------------------
float SwarmScene::SceneOutro() {
	cout << "Swarm Outro Triggered" << endl;
	return 0.5f;
}