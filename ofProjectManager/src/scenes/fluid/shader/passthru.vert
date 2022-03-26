#version 150

in vec4 position;
in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;

out vec2 vTexCoord;

void main() {

    vTexCoord = texcoord;
    gl_Position = modelViewProjectionMatrix * vec4(position);
}
