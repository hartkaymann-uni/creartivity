#version 150

uniform sampler2DRect velocity;

uniform vec2 gridSize;
uniform float gridScale;

out vec4 vFragColor;

void main() {
	vec2 coords = gl_FragCoord.xy;

	vec2 xOffset = vec2(1.0, 0.0);
	vec2 yOffset = vec2(0.0, 1.0);

	float vl = texture(velocity,  coords - xOffset).y;
	float vr = texture(velocity,  coords + xOffset).y;
	float vb = texture(velocity,  coords - yOffset).x;
	float vt = texture(velocity,  coords + yOffset).x;

	float scale = 0.5 / gridScale;

	vFragColor = vec4(scale * (vr - vl - vt + vb), 0.0, 0.0, 1.0);
}