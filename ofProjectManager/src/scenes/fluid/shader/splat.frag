#version 150

uniform sampler2D read;

uniform vec2 gridSize;

uniform vec3 color;
uniform vec2 point;
uniform float radius;

out vec4 vFragColor;

float gauss(vec2 p, float r) 
{
	return exp(-dot(p, p) / r);
}

void main() 
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;
	vec3 base = texture(read, uv).xyz;
	vec2 coord = point.xy - uv;
    vec3 splat = color * gauss(coord, radius);
	
	vFragColor = vec4(base + vec3(gauss(coord, radius) - .1), 1.0);
}