#version 150

in vec2 vTexCoord;

uniform sampler2DRect b; // b vector (Ax = b)
uniform sampler2DRect x; // x vector (Ax = b)

uniform float timestep;
uniform float alpha;
uniform float rBeta;

out vec4 vFragColor;

void main() {

	// left, right, bottom and top samples
	vec4 xL = texture(x, vTexCoord - vec2(1, 0));
	vec4 xR = texture(x, vTexCoord + vec2(1, 0));
	vec4 xB = texture(x, vTexCoord - vec2(0, 1));
	vec4 xT = texture(x, vTexCoord + vec2(0, 1));

	// b sample, from center
	vec4 bC = texture(b, vTexCoord);

	// evaluate Jacobi iteration
	vFragColor = (xL + xR + xB + xT + alpha * bC) * rBeta;
}