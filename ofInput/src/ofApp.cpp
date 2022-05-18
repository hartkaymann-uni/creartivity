#include "ofApp.h"


void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	ofBackground(0);

	width = ofGetWidth();
	height = ofGetHeight();

	sender.setup(HOST, PORT);

	sendConnectionStarted();

	gui.setup();
	gui.add(add_button.setup("Add user", 30, 30));
	gui.add(remove_button.setup("Remove user", 30, 30));
	gui.add(isSequencerInControl.set("Let Sequencer control", true));

	gui.setPosition(width - gui.getWidth() - 1, height - gui.getHeight() - 10);
	add_button.addListener(this, &ofApp::handleAddButtonClick);
	remove_button.addListener(this, &ofApp::handleRemoveButtonClick);
	isSequencerInControl.addListener(this, &ofApp::OnSequencerControlChange);

	//InitTestSeqeuenceArray();
	currentSequenceIndex = 0;
	sequences.push_back(ParameterSequence(10000, SequenceName::Smooth));
	SetSequence(sequences[currentSequenceIndex]);

	lastMousePos = unmapped(glm::vec2(0.4f, 0.7f));


	// Register first user
	registerUser(0);
}

void ofApp::update() {
	// Iterate through users and send information
	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		sendUser(it->first, it->second);
		it++;
	}

	UpdateSequence();
}

void ofApp::draw() {
	ofClear(0);
	ofBackground(0);
	int radius = 10;

	// Iterate through users and draw positions
	ofPushStyle();
	ofNoFill();
	std::map<int, user>::iterator it = users.begin();
	std::map<int, user>::iterator itEnd = users.end();
	while (it != itEnd) {
		ofSetColor(getUserColor(it->second.id));
		ofDrawCircle(unmapped(it->second.left), 10);
		ofDrawCircle(unmapped(it->second.right), 10);
		it++;
	}
	ofPopStyle();
	gui.draw();
}

// Send user information
void ofApp::sendUser(int id, user& user) {
	ofxOscMessage m;
	std::string addr = "/user/data/";
	addr += ofToString(id);
	m.setAddress(addr);

	// Position left hand
	m.addFloatArg(user.left.x);
	m.addFloatArg(user.left.y);

	// Position rigt hand
	m.addFloatArg(user.right.x);
	m.addFloatArg(user.right.y);

	sender.sendMessage(m);
}

// Register a user
void ofApp::registerUser(int id)
{
	user u = { id, {0.f, 0.f}, {0.f, 0.f} };
	users[id] = u;
	// Update UI
	mouseDragged(lastMousePos.x, lastMousePos.y, 0);

	ofxOscMessage m;
	m.setAddress("/user/new/");
	m.addInt32Arg(id);
	sender.sendMessage(m);
}

// Remove one user
void ofApp::removeUser(int id)
{
	map<int, user>::iterator it = users.find(id);
	if (it != users.end())
		users.erase(it);

	ofxOscMessage m;
	m.setAddress("/user/lost/");
	m.addInt32Arg(id);
	sender.sendMessage(m);
}

// Sent once upon establishing a connection
void ofApp::sendConnectionStarted() {
	ofxOscMessage m;
	m.setAddress("/connection");
	m.addStringArg("on");
	m.addStringArg(HOST);
	m.addInt32Arg(PORT);
	sender.sendMessage(m);
}

void ofApp::addNewUsers(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		int id = users.size();
		registerUser(id);
	}
}

void ofApp::removeMostRecentUser(bool keepLastUser)
{
	int id = users.size() - 1;
	if (keepLastUser == true) {
		if (id > 0) removeUser(id);
	}
	else if (id >= 0) removeUser(id);
}

void ofApp::RemoveAllUsers()
{
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		removeMostRecentUser(false);
	}
}

void ofApp::handleAddButtonClick()
{
	addNewUsers(1);
}

void ofApp::handleRemoveButtonClick()
{
	removeMostRecentUser();
}

void ofApp::mouseDragged(int x, int y, int button) {
	lastMousePos = glm::vec2(x, y);

	if (isSequencerInControl.get() == false) {
		createUserPattern(lastMousePos.x, lastMousePos.y);
	}
}

// Arrange user hand positions in a pattern
void ofApp::createUserPattern(int x, int y) {
	glm::vec2 coords = mapped({ x, y });

	// Arrange users in pattern
	glm::vec2 center = { width * 0.5f, height * 0.5f };
	float radius = glm::length(center - glm::vec2(x, y));
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		float offset = 0.1f * ceil(i / 2.0) * (i % 2 ? 1.f : -1.f);
		users[i].left = coords + glm::vec2{ offset };
		//users[i].right = glm::vec2(1.f) - coords + glm::vec2{ offset };
	}
}

void ofApp::windowResized(int w, int h) {
	width = w;
	height = h;
}

// Return normalized coordinates from screen coordinates
glm::vec2 ofApp::mapped(glm::vec2 v)
{
	glm::vec2 mapped;
	mapped.x = ofMap(v.x, 0.f, width, 0.f, 1.f, true);
	mapped.y = ofMap(v.y, 0.f, height, 0.f, 1.f, true);
	return mapped;
}

// Return screen coordinates from normalized coordinates
glm::vec2 ofApp::unmapped(glm::vec2 v)
{
	glm::vec2 unmapped;
	unmapped.x = ofMap(v.x, 0.f, 1.f, 0.f, width, true);
	unmapped.y = ofMap(v.y, 0.f, 1.f, 0.f, height, true);
	return unmapped;
}

// ######################
// CodeSection: Sequencer
// ######################

void ofApp::InitTestSeqeuenceArray() {
	currentSequenceIndex = 0;
	sequences.push_back(ParameterSequence(5, SequenceName::Smooth));
	sequences.push_back(ParameterSequence(10, SequenceName::UserChaos));
	sequences.push_back(ParameterSequence(5, SequenceName::NoUsers));
	sequences.push_back(ParameterSequence(5, SequenceName::Edging));
	sequences.push_back(ParameterSequence(5, SequenceName::Crazy));
	sequences.push_back(ParameterSequence(5, SequenceName::OutOfBounds));
	sequences.push_back(ParameterSequence(5, SequenceName::RandomTeleport));
	sequences.push_back(ParameterSequence(5, SequenceName::SmoothCenter));
	sequences.push_back(ParameterSequence(5, SequenceName::Fast));
	SetSequence(sequences[currentSequenceIndex]);
}

// Sets currentSequence to a new sequence and immediately activates it.
void ofApp::SetSequence(ParameterSequence sequence) {
	currentSequence = sequence;
	StartSequence();
}

// Activates the current sequence by setting the intial parameters
void ofApp::StartSequence() {
	lastSequenceTime = ofGetElapsedTimef();
	nextSequenceTime = lastSequenceTime + currentSequence.duration;

	RemoveAllUsers();
	addNewUsers(3);

	switch (currentSequence.sequenceType)
	{
	case SequenceName::NoUsers:
		RemoveAllUsers();
		break;
	case SequenceName::Edging:
		ApplyPerlinMovement();
		AssignRandomEdges();
		break;
	case SequenceName::OutOfBounds:
		ApplyPerlinMovement();
		AssignRandomOutOfBounds();
		break;
	default:
		break;
	}

	UpdateSequence();
}

// If nextSequenceTime has arrived, activate the next sequence in the sequences vector
void ofApp::CheckForNextSequence() {
	if (ofGetElapsedTimef() > nextSequenceTime) {
		currentSequenceIndex++;
		if (currentSequenceIndex >= sequences.size() || currentSequenceIndex < 0) currentSequenceIndex = 0;

		SetSequence(sequences[currentSequenceIndex]);
		cout << "Input-Tester switched to next sequence with index " << currentSequenceIndex << " . Will wait for " << sequences[currentSequenceIndex].duration << " seconds now." << endl;
	}
}

// Change the parameters over time according to the current scene
void ofApp::UpdateSequence() {
	if (isSequencerInControl.get() == false) return;

	CheckForNextSequence();

	switch (currentSequence.sequenceType)
	{
	case SequenceName::Smooth:
		ApplyPerlinMovement(0.5f, 2.5f);
		break;
	case SequenceName::SmoothCenter:
		ApplyPerlinMovement();
		break;
	case SequenceName::Crazy:
		ApplyPerlinMovement(1.0f, 2.5f);
		break;
	case SequenceName::Fast:
		ApplyPerlinMovement(2.0f, 1.0f);
		break;
	case SequenceName::RandomTeleport:
		if (ofRandom(1.0f) < 0.2f) TeleportToRandomLocation();
		break;
	case SequenceName::Edging:
		MoveToTarget();
		break;
	case SequenceName::OutOfBounds:
		MoveToTarget();
		break;
	case SequenceName::UserChaos:
		if ((ofRandom(1.0f) > 0.48f) && users.size() <= 25) addNewUsers(1);
		else removeMostRecentUser();

		ApplyPerlinMovement(1.25f, 2.0f);
		break;
	default:
		break;
	}
}

void ofApp::OnSequencerControlChange(bool& inControl)
{
	if (inControl == false) {
		createUserPattern(lastMousePos.x, lastMousePos.y);
	}
}


// ##########################
// CodeSection: User Movement
// ##########################

void ofApp::ApplyPerlinMovement(float speedMod, float exaggerateMod)
{
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		float offset = i * 1.378f;

		float x = ofNoise(ofGetElapsedTimef() * 0.8 * speedMod + offset);
		float y = ofNoise(ofGetElapsedTimef() * 0.7 * speedMod + offset + 0.4);

		users[i].left = ExaggerateMovement(glm::vec2(x, y), exaggerateMod);

		// Different Movement for right hand of user
		x = ofNoise(ofGetElapsedTimef() * 0.2 * speedMod + offset + 0.6);
		y = ofNoise(ofGetElapsedTimef() * 0.4 * speedMod + offset + 0.3);

		users[i].right = ExaggerateMovement(glm::vec2(x, y), exaggerateMod);

	}
}

// Pushes a point more towards the boundaries of the box
glm::vec2 ofApp::ExaggerateMovement(glm::vec2 point, float mod) {
	float splitAt = 0.5f;

	point.x = (point.x - splitAt) * mod;
	//point.x = min(max(point.x, -splitAt), splitAt) + splitAt;
	point.x += splitAt;

	point.y = (point.y - splitAt) * mod;
	//point.y = min(max(point.y, -splitAt), splitAt) + splitAt;
	point.y += splitAt;

	return point;
}

// Teleports all users to a random position that can also be out of the bounds of the input area
void ofApp::TeleportToRandomLocation()
{
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		users[i].left = glm::vec2(ofRandom(1.5f) - 0.25f, ofRandom(1.5f) - 0.25f);
		users[i].right = glm::vec2(ofRandom(1.5f) - 0.25f, ofRandom(1.5f) - 0.25f);
	}
}

void ofApp::AssignRandomOutOfBounds()
{
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		userTargets[i] = glm::vec2(4 * (rand() % 2 - 0.5f), 4 * (rand() % 2 - 0.5f));
	}
}

void ofApp::AssignRandomEdges()
{
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		userTargets[i] = glm::vec2(rand() % 2, rand() % 2);
	}
}

void ofApp::MoveToTarget()
{
	int n = users.size();
	for (size_t i = 0; i < n; i++) {
		MoveUserTowardsPoint(i, userTargets[i]);
	}
}

// Point is expected to be in normalized coords.
void ofApp::MoveUserTowardsPoint(int id, glm::vec2 point)
{
	// Check if user exists
	map<int, user>::iterator it = users.find(id);
	if (it == users.end()) return;

	// Framerate independent movement speed
	float speed = (0.2f + 0.05f * id) * ofGetLastFrameTime();

	glm::vec2 diff = point - users[id].left;
	users[id].left += glm::normalize(diff) * speed;

	diff = point - users[id].right;
	users[id].right += glm::normalize(diff) * speed;
}



// ##########################
// CodeSection: Visualisation
// ##########################

// Gets a unique user color for circles
ofColor ofApp::getUserColor(int id)
{
	map<int, ofColor>::iterator it = userColors.find(id);
	if (it != userColors.end()) {
		return it->second;
	}

	// If not found, return white
	return ofColor(255, 255, 255);
}