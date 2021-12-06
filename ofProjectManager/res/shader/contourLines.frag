#version 150

in vec4 vPosition;
in vec4 vColor;

out vec4 vFragColor;


void main() {

	float c = 1.0 - mod(vPosition.z * 100 , 50);

	vFragColor = vec4(c);

}