#version 150

in vec2 vTexCoord;

uniform sampler2DRect u;

uniform bool mouseDown;
uniform vec3 mousePos;
uniform float mouseRad;
uniform float mouseStr;

out vec4 vFragColor;

void main() {

	vFragColor = vec4(0.f);
}