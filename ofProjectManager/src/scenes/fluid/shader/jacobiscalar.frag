#version 150

uniform sampler2DRect b;
uniform sampler2DRect x;

uniform vec2 gridSize;

uniform float alpha;
uniform float beta;

out vec4 vFragColor;

void main() {
	vec2 coords = gl_FragCoord.xy; 
	vec2 uv = coords / gridSize.xy;

	vec2 xOffset = vec2(1.0, 0.0);
	vec2 yOffset = vec2(0.0, 1.0);

	// left, right, bottom and top samples
	float xl = texture(x,  coords + xOffset).x;
	float xr = texture(x,  coords + yOffset).x;
	float xb = texture(x,  coords - xOffset).x;
	float xt = texture(x,  coords - yOffset).x;

	// b sample, from center
	float bc = texture(b, coords).x;

	// evaluate Jacobi iteration
	vFragColor = vec4((xl + xr + xb + xt + alpha * bc) / beta, 0.0, 0.0, 1.0);
}