#version 150

uniform sampler2DRect cellTex;

in vec2 vTexCoord;
in vec4 vColor;

out vec4 vFragColor;

void main() {
    vFragColor = vec4(1.0, 0.0, 1.0, 1.0);
}