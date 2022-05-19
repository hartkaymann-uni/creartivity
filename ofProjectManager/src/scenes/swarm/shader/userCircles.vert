
#version 440

in vec4 position;
in vec4 color;

uniform mat4 modelViewProjectionMatrix;

out vec4 fragColor;



void main(){
    gl_Position = modelViewProjectionMatrix * position;

    fragColor = color;
}