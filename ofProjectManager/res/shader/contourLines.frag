#version 150

in vec4 vPosition;
in vec4 vColor;

out vec4 vFragColor;


void main() {
	float c = 1.0 - round(mod(vPosition.z*10, 20));
	if(vPosition.z <= 0.5) c = 0.0;

	float r = abs(vPosition.z / 5);

	vFragColor = vec4(c);

}