#version 150

in vec2 vTexCoord;

uniform sampler2D read;

uniform vec2 gridSize;

uniform vec3 bias;
uniform vec3 scale;

out vec4 vFragColor; 

void main()
{
    vFragColor = vec4(bias + scale * texture(read, vTexCoord).xxx, 1.0);
}