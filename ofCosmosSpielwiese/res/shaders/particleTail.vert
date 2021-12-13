#version 150

in vec4 position;

uniform vec4 globalColor;
uniform mat4 modelViewProjectionMatrix;

out vec4 fragColor;


void main(){

    fragColor = globalColor;
    gl_Position = modelViewProjectionMatrix * position;
}