#version 150

uniform sampler2DRect cellTex;

uniform vec2 screen;
uniform float size;


flat in vec4 vColor;

out vec4 vFragColor;

void main() {

	vFragColor = vColor;
}