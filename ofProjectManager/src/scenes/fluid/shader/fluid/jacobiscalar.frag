#version 150

uniform sampler2D b;
uniform sampler2D x;

uniform vec2 gridSize;

uniform float alpha;
uniform float beta;

out vec4 vFragColor;

void main() 
{
	vec2 uv = gl_FragCoord.xy / gridSize.xy;

    vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
    vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

	// left, right, bottom and top samples
	float xl = texture(x,  uv + xOffset).x;
	float xr = texture(x,  uv + yOffset).x;
	float xb = texture(x,  uv - xOffset).x;
	float xt = texture(x,  uv - yOffset).x;

	// b sample, from center
	float bc = texture(b, uv).x;

	// evaluate Jacobi iteration
	vFragColor = vec4((xl + xr + xb + xt + alpha * bc) / beta, 0.0, 0.0, 1.0);
}