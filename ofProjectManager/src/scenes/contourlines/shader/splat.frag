#version 150

uniform sampler2D read;

uniform vec2 grid;

uniform vec2 left;
uniform vec2 right;
uniform float radius;
uniform float strength;

out vec4 vFragColor;

float gauss(vec2 p, float r) 
{
	return exp(-dot(p, p) / r);
}

void main() 
{
    vec2 uv = gl_FragCoord.xy / grid.xy;
	vec3 base = texture( read, uv ).xyz;
	
	vec2 l = left.xy - uv;
	vec2 r = right.xy - uv;
    
	vec3 splatL = vec3( gauss( l, radius ) ) * strength;
	vec3 splatR = vec3( gauss( r, radius ) ) * strength;
	
	vFragColor = vec4( clamp(base + splatL + splatR, vec3(0, 0, 0), vec3(1, 1, 1)), 1.f );
	//vFragColor = vec4( clamp(base.xy + left - right, vec2(0, 0), vec2(1, 1)), base.z, 1.f);
}