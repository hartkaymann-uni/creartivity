#version 150

in vec2 vTexCoord;

uniform sampler2DRect read;

uniform vec2 gridSize;

uniform vec3 color;
uniform vec2 point;
uniform float radius;

out vec4 vFragColor;

float gauss(vec2 p, float r) {
	return exp(-dot(p, p) / r);
}

void main() {

	vec2 coords = gl_FragCoord.xy;

	vec3 base = texture(read, coords).xyz;

	vec2 coord = point.xy - gl_FragCoord.xy;
	vec3 splat = color * gauss(coord, gridSize.x * radius);

	vFragColor = vec4(base + splat, 1.0);
}