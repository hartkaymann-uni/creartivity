#version 150

in vec2 vTexCoord;

uniform sampler2DRect w; // vector field

uniform float rdx; // gridscale / 0.5 

out vec4 vFragColor; // divergence

void main() {
	vec4 wL = texture(w, vTexCoord - vec2(1, 0));
	vec4 wR = texture(w, vTexCoord + vec2(1, 0));
	vec4 wB = texture(w, vTexCoord - vec2(0, 1));
	vec4 wT = texture(w, vTexCoord + vec2(0, 1));

	vFragColor = rdx * vec4((wR.x - wL.x) + (wT.y - wB.y));
}