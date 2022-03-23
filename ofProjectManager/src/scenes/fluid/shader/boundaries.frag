#version 150

in vec2 vTexCoord;

uniform sampler2DRect x; // state field

uniform vec2 offset;

uniform float scale; 

out vec4 vFragColor; 

void main() {
	vFragColor = scale * texture(x, vTexCoord + offset);
}