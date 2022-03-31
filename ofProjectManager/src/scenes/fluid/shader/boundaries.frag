#version 150

uniform sampler2DRect read;

uniform vec2 gridSize;
uniform vec2 gridOffset;
uniform float scale; 

out vec4 vFragColor; 

void main() {
	vec2 coords = (gl_FragCoord.xy + gridOffset.xy);
	vFragColor = vec4(scale * texture(read, coords).xyz, 1.0);
	//vFragColor = vec4(0.0, -1.0, 0.0, 1.0);
}