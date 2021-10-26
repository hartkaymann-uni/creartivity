#include "ofApp.h"


void ofApp::setup(){

    filesystem::path shader_path( "../../res/shaders" );
    shader.load( shader_path / "gameOfLife.vert", shader_path / "gameOfLife.frag" );

    float planeScale = 0.75;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;

    plane.set( planeWidth, planeHeight, 1, 1, OF_PRIMITIVE_LINES);
}

void ofApp::update(){

}

void ofApp::draw(){

    shader.begin();

    shader.setUniformTexture("tex0", )
    shader.end();


}
