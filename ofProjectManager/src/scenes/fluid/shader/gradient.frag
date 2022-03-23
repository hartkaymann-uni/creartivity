#version 150

in vec2 vTexCoord;

uniform sampler2DRect w; // velocity
uniform sampler2DRect p; // pressure

uniform float rdx; // gridscale / 0.5

out vec4 vFragColor; // new velocity

void main() {
	float pL = texture(w, vTexCoord - vec2(1, 0)).x;
	float pR = texture(w, vTexCoord + vec2(1, 0)).x;
	float pB = texture(w, vTexCoord - vec2(0, 1)).x;
	float pT = texture(w, vTexCoord + vec2(0, 1)).x;

	vFragColor = texture(w, vTexCoord);
	vFragColor.xy -= rdx * vec2(pR - pL, pT - pB);
}