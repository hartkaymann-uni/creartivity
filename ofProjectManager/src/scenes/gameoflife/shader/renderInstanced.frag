#version 150

uniform float size;
uniform float radius;

in vec4 vPosition;
flat in vec4 vColor;
in vec3 vNormal;

out vec4 vFragColor;

void main() {
	// Set brightness according to z value
	float z = smoothstep(0.1, 0.75, pow(vPosition.z / (radius * size), 1.0));
	vFragColor = vec4( 0.0, 0.0, 0.0, 1.0);
}