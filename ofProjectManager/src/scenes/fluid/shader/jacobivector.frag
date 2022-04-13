#version 150

uniform sampler2DRect x;
uniform sampler2DRect b;

uniform vec2 gridSize;

uniform float alpha;
uniform float beta;

out vec4 vFragColor;

void main() {
	vec2 coords = gl_FragCoord.xy;

	vec2 xOffset = vec2(1.0, 0.0);
	vec2 yOffset = vec2(0.0, 1.0);

	// left, right, bottom and top samples
	vec4 xl = texture(x,  coords + xOffset);
	vec4 xr = texture(x,  coords + yOffset);
	vec4 xb = texture(x,  coords - xOffset);
	vec4 xt = texture(x,  coords - yOffset);

	// b sample, from center
	vec4 bc = texture(b, coords);

	// evaluate Jacobi iteration
	vFragColor = (xl + xr + xb + xt + alpha * bc) / beta;
}