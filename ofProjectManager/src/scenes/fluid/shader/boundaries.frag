#version 150

uniform sampler2DRect read;

uniform vec2 gridSize;
uniform vec2 gridOffset;
uniform float scale; 

out vec4 vFragColor; 

void main() {
	vec2 uv = (gl_FragCoord.xy + gridOffset.xy) / gridSize.xy;
	vFragColor = vec4(scale * texture(read, uv).xyz, 1.0);
}