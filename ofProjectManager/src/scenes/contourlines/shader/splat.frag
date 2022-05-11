#version 150

uniform sampler2D read;

uniform vec2 grid;

uniform vec2 left;
uniform vec2 right;
uniform float radius;

out vec4 vFragColor;

float gauss(vec2 p, float r) 
{
	return exp(-dot(p, p) / r);
}

void main() 
{
    vec2 uv = gl_FragCoord.xy / grid.xy;
	vec3 base = texture(read, uv).xyz;
	vec2 coord = point.xy - uv;
    vec3 splat = color * gauss(coord, radius);
	
	vFragColor = vec4(base + splat, 1.0);
}