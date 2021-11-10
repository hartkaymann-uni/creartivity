#version 150

uniform sampler2DRect cellData; // Previous generation cell data

uniform float timestep;

in vec2 vTexCoord;

out vec4 vFragColor;

void main(void){
    // Get the cell state pixel color.
    vec2 state = texture( cellData, vTexCoord ).xy;

    float shift_r = sin(timestep) * vTexCoord.x/255;
    float shift_g = cos(timestep) * vTexCoord.y/255;
    float shift_b = 0;
            
    // And finally store it on the position FBO.
    vFragColor = vec4(shift_r, shift_g, shift_b, 1.0);
}