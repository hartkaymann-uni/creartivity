#version 150

uniform sampler2D read;

uniform float threshhold;

in vec2 vTexCoord;

out vec4 vFragColor;

void main() 
{
    vec3 col = texture(read, vTexCoord).xyz;
    float brightness = length(col);

    vFragColor = vec4( step(threshhold, brightness) * col, 1.0);
    //vFragColor = vec4(vTexCoord, vTexCoord);
}