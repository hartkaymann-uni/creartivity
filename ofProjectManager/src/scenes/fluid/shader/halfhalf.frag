#version 150

in vec2 vTexCoord;

out vec4 vFragColor;

void main() {
	float col = vTexCoord.y > 0.5 ? 0.0 : 1.0;
	vFragColor = vec4(vec3(col), 1.0);
}