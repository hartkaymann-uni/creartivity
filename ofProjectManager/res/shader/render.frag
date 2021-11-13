#version 150

uniform sampler2DRect cellTex;

in vec2 vTexCoord;
in vec4 vColor;

out vec4 vFragColor;

void main() {
    vec4 col = texture( cellTex, vTexCoord );

    vFragColor = col;
}