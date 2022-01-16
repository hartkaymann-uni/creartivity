#version 150

out vec4 vFragColor;

flat in vec4 vColor;

void main() {

	vFragColor = vColor;
}