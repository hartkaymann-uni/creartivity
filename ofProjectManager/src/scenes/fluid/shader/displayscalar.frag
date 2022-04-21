#version 150

in vec2 vTexCoord;

uniform sampler2D read;

uniform vec2 gridSize;

uniform vec3 bias;
uniform vec3 scale;

out vec4 vFragColor; 

void main()
{
    vec3 c = texture(read, vTexCoord).xyz;
    //c = floor(c + vec3(0.7));

    vFragColor = vec4(c, 1.0);
}