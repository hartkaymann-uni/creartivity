#version 150

uniform sampler2D read;

uniform vec2 grid;

uniform vec2 u_point;
uniform float u_radius;
uniform float u_strength;

out vec4 vFragColor;

float gauss(vec2 p, float r) 
{
	p.x *= grid.x / grid.y;
	return exp(-dot(p, p) / r);
}

void main() 
{
    vec2 uv = gl_FragCoord.xy / grid.xy;
	float base = texture( read, uv ).x;
	
	vec2 p = u_point.xy - uv;
    
	float splat = gauss( p, u_radius ) * u_strength ;
	
	float col = clamp(base + splat, 0.0, 1.0);
	vFragColor = vec4( col, 0.0, 0.0, 1.0 );
	//vFragColor = vec4(uv, 0.0, 1.0);
}