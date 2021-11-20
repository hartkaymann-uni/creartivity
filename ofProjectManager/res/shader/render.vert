#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
in vec4 color;

uniform sampler2DRect cellTex;
uniform vec2 screen;
uniform vec2 resolution;

flat out vec4 vPosition;
flat out vec2 vTexCoord;
flat out vec4 vColor;

void main() {

    vPosition = modelViewProjectionMatrix * position;
    vTexCoord = texcoord;
    vColor = vec4(0.1, 0.2, 0.6, 1.0);

    gl_Position = modelViewProjectionMatrix * position;
}