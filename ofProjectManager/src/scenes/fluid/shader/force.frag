#version 150

in vec2 vTexCoord;

uniform sampler2DRect u;

uniform bool mouseDown;
uniform vec3 mousePos;
uniform float mouseRad;
uniform float mouseStr;

out vec4 vFragColor;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {

	float d = distance(vTexCoord, mousePos.xy);
	float c = map(d, 0.0, 100.0, 0.0, 0.5);

	vFragColor = vec4(c);
}