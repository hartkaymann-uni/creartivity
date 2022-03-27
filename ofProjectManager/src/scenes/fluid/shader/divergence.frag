#version 150

uniform sampler2DRect velocity;

uniform vec2 gridSize;
uniform float gridScale;

out vec4 vFragColor; 

void main() {
	vec2 uv = gl_FragCoord.xy / gridSize.xy;

	vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
	vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

	float vl = texture(velocity, uv - xOffset).x;
	float vr = texture(velocity, uv + xOffset).x;
	float vb = texture(velocity, uv - yOffset).x;
	float vt = texture(velocity, uv + yOffset).x;

	float scale = 0.5 / gridScale;
	float divergence = scale * (vr - vl + vt - vb);

	vFragColor = vec4(divergence, 0.0, 0.0, 1.0);
}