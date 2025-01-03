#version 150

in vec2 vTexCoord;

uniform sampler2D x;
uniform sampler2D b;

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
	vec2 xl = texture(x,  uv + xOffset).xy;
	vec2 xr = texture(x,  uv + yOffset).xy;
	vec2 xb = texture(x,  uv - xOffset).xy;
	vec2 xt = texture(x,  uv - yOffset).xy;

	// b sample, from center
	vec2 bc = texture(b, uv).xy;

	// evaluate Jacobi iteration
	vFragColor = vec4((xl + xr + xb + xt + alpha * bc) / beta, 0.0, 1.0);
}