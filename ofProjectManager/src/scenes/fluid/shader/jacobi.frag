#version 150

in vec2 vTexCoord;

out vec4 vFragColor;
uniform sampler2DRect cellData;

void main() {

	vFragColor = texture(cellData, vTexCoord);
}