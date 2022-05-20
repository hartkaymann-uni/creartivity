#version 150

uniform sampler2D read;

uniform vec2 grid;

out vec4 vFragColor;

void main() 
{
    vec2 uv = gl_FragCoord.xy / grid.xy;
	float base = texture( read, uv ).x;
	
	//vFragColor = vec4( vec3(pow(base * 0.999, 1.1)), 1.f );
	vFragColor = vec4( vec3((base * 0.99)), 1.f );
	//vFragColor = vec4( uv, uv);	
}