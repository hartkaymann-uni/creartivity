#version 150

uniform sampler2D read;
uniform sampler2D velocity;

uniform vec2 resolution;

in vec2 vTexCoord;

out vec4 vFragColor; 

void main()
{
    vec2 v = texture(velocity, vTexCoord).xy / resolution.xy;
    vec4 base = texture(read, vTexCoord - v); 

    vFragColor = base;
    //vFragColor  = vec4(vTexCoord, vTexCoord);
}
