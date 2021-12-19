#version 430

in vec4 position;

uniform mat4 modelViewProjectionMatrix;


void main(){
    gl_Position = modelViewProjectionMatrix * position;
    //gl_Position = vec4(1.0, 1.0, 1.0, 0.0);
}