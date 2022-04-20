#version 150

in vec2 vTexCoord;

uniform sampler2D read;

uniform vec2 gridSize;
uniform vec2 gridOffset;
uniform float scale; 

out vec4 vFragColor;

void main() {
	vec2 uv = (gl_FragCoord	.xy + gridOffset.xy) / gridSize.xy;
    gl_FragColor = vec4(scale * texture(read, uv).xyz, 1.0);
	vFragColor = vec4(0.0, 1.0, 1.0, 1.0);
}