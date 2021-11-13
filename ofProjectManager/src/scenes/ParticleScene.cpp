#include "ParticleScene.h"


ParticleScene::ParticleScene() : ParticleScene(10) {}

ParticleScene::ParticleScene( int n_particles ) : ofxFadeScene( "Particles" ) {
	setSingleSetup( false );
	setFade( 1000, 1000 );
}

//--------------------------------------------------------------
void ParticleScene::setup() {
	filesystem::path res_path( "../../res/" );
	compute.setupShaderFromFile( GL_COMPUTE_SHADER, res_path / "shader/particle.comp" );
	bool linked = compute.linkProgram();

	bool renderLinked = renderShader.load( res_path / "shader/instancedParticles.vert", res_path / "shader/instancedParticles.frag" );

	camera.disableMouseInput();
	camera.setupPerspective();
	camera.setPosition( 0, 0, 665 );
	camera.setFarClip( ofGetWidth() * 10 );

	particles.resize( 1024 * 8 * 1 );
	int i = 0;
	for (auto& p : particles)
	{
		p.pos.x = ofRandom( -ofGetWidth() * 0.5, ofGetWidth() * 0.5 );
		p.pos.y = ofRandom( -ofGetHeight() * 0.5, ofGetHeight() * 0.5 );
		p.pos.z = 0;
		p.pos.w = 1;
		p.vel = { 0,0,0,0 };
		i++;
	}
	particlesBuffer.allocate( particles, GL_DYNAMIC_DRAW );
	particlesBuffer2.allocate( particles, GL_DYNAMIC_DRAW );

	vbo.setVertexBuffer( particlesBuffer, 4, sizeof( Particle ) );
	vbo.setColorBuffer( particlesBuffer, sizeof( Particle ), sizeof( glm::vec4 ) * 2 );
	vbo.enableColors();
	m_DirAsColor = true;

	ofBackground( 0 );
	ofEnableBlendMode( OF_BLENDMODE_ADD );

	m_NoiseShift.set( 0 );
	m_MousePressed.set( "Mouse Pressed", false );
	m_DrawArrows.set( "Draw Arrows", false );
	m_MaxSpeed.set( "u_MaxSpeed", 2500, 0, 5000 );
	m_DeltaNoiseShift.set( "Noise Shift", 0.001, 0.0, 1.0 );

	gui.setup();
	m_ShaderUniforms.setName( "Shader Parameters" );
	m_ShaderUniforms.add( m_MaxSpeed );
	gui.add( m_ShaderUniforms );
	gui.add( m_DrawArrows );
	gui.add( m_DeltaNoiseShift );
	gui.add( fps.set( "fps", 60, 0, 60 ) );


	particlesBuffer.bindBase( GL_SHADER_STORAGE_BUFFER, 0 );
	particlesBuffer2.bindBase( GL_SHADER_STORAGE_BUFFER, 1 );

	for (int x = 0; x < FIELD_X; x++) {
		for (int y = 0; y < FIELD_Y; y++) {
			noiseField[x][y] = ofNoise( glm::vec3( x, y, m_NoiseShift ) );
		}
	}


	// Create VBO Mesh
	ofSpherePrimitive sphere;
	sphere.set( 10, 5 );
	m_VboMesh = sphere.getMesh();

	// Create ground grid mesh
	int groundSize = 25;
	groundGrid.setMode( OF_PRIMITIVE_POINTS );
	for (int x = 0; x < groundSize; x++) {
		for (int z = 0; z < groundSize; z++) {
			int vPosX = std::ceil( -groundSize / 2 ) + x;
			int vPosZ = std::ceil( -groundSize / 2 ) + z;
			groundGrid.addVertex( { vPosX, 0, vPosZ } );
		}
	}
}

//--------------------------------------------------------------
void ParticleScene::update() {
	fps = ofGetFrameRate();

	compute.begin();
	compute.setUniforms( m_ShaderUniforms );
	compute.setUniform1f( "u_TimeLastFrame", ofGetLastFrameTime() );
	compute.setUniform1f( "u_ElapsedTime", ofGetElapsedTimef() );
	compute.setUniform1f( "u_ScreenWidth", ofGetScreenWidth() );
	compute.setUniform1f( "u_ScreenHeight", ofGetScreenHeight() );
	compute.setUniform1f( "u_NoiseShift", m_NoiseShift );
	compute.setUniform3f( "u_MousePosition", m_MousePosition );
	compute.setUniform1i( "u_MousePressed", m_MousePressed );

	compute.dispatchCompute( (particles.size() + 1024 - 1) / 1024, 1, 1 );
	compute.end();

	particlesBuffer.copyTo( particlesBuffer2 );

	m_NoiseShift += m_DeltaNoiseShift;
	for (int x = 0; x < FIELD_X; x++) {
		for (int y = 0; y < FIELD_Y; y++) {
			noiseField[x][y] = ofNoise( glm::vec3( x, y, m_NoiseShift ) );
		}
	}
	
}

//--------------------------------------------------------------
void ParticleScene::draw() {
	//ofEnableBlendMode( OF_BLENDMODE_ADD );
	ofEnableDepthTest();
	ofDisableAlphaBlending();

	camera.begin();
	ofSetColor( 255, 70 );
	glPointSize( 2 );
	vbo.draw( GL_POINTS, 0, particles.size() );

	ofNoFill();
	ofDrawBox( 0, 0, -ofGetHeight() * 2, ofGetWidth() * 4, ofGetHeight() * 4, ofGetHeight() * 4 );
	groundGrid.drawWireframe();

	ofFill();
	ofSetColor( ofColor::blue );
	ofDrawBox( glm::vec3( 0.0 ), 5 );

	// Draw call for instanced objects
	renderShader.begin();
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	//m_VboMesh.drawInstanced( OF_MESH_FILL, 1024 * 8 * 1 );
	
	glDisable( GL_CULL_FACE );
	renderShader.end();

	camera.end();

	ofDisableDepthTest();
	ofEnableAlphaBlending();
	ofEnableBlendMode( OF_BLENDMODE_ALPHA );
	
	ofSetColor( 255 );
	gui.draw();
}

//--------------------------------------------------------------
void ParticleScene::keyPressed( int key ) {

	// std::cout << key << std::endl;
	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.enableMouseInput();
	}
}

//--------------------------------------------------------------
void ParticleScene::keyReleased( int key ) {

	if (key == ofKey::OF_KEY_SHIFT)
	{
		camera.disableMouseInput();

	}
}

//--------------------------------------------------------------
void ParticleScene::mouseMoved( int x, int y ) {

}

//--------------------------------------------------------------
void ParticleScene::mouseDragged( int x, int y, int button ) {
	if (m_MousePressed)
		m_MousePosition = glm::vec3( x, y, 0 );
}

//--------------------------------------------------------------
void ParticleScene::mousePressed( int x, int y, int button ) {
	m_MousePressed = true;
	m_MousePosition = glm::vec3( x, y, 0 );
}

//--------------------------------------------------------------
void ParticleScene::mouseReleased( int x, int y, int button ) {
	m_MousePressed = false;
}

//--------------------------------------------------------------
void ParticleScene::mouseEntered( int x, int y ) {

}

//--------------------------------------------------------------
void ParticleScene::mouseExited( int x, int y ) {

}

//--------------------------------------------------------------
void ParticleScene::windowResized( int w, int h ) {

}

//--------------------------------------------------------------
void ParticleScene::gotMessage( ofMessage msg ) {

}

//--------------------------------------------------------------
void ParticleScene::dragEvent( ofDragInfo dragInfo ) {

}
