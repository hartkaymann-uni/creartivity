#version 150

uniform sampler2D read;

uniform vec2 grid;

out vec4 vFragColor;

void main() 
{
    vec2 uv = gl_FragCoord.xy / grid.xy;
	vec3 base = texture( read, uv ).xyz;
	
	vFragColor = vec4( base * 0.99f, 1.f );
	//vFragColor = vec4( uv, uv);	
}