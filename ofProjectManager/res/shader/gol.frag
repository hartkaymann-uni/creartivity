#version 150

uniform sampler2DRect cellData; // Previous generation cell data

uniform float timestep;

in vec2 vTexCoord;

out vec4 vFragColor;

void main(void){
    // Get the cell state pixel color.
    vec2 pos = texture( cellData, vTexCoord ).xy;
    
    // Update the position.
    
    // And finally store it on the position FBO.
    vFragColor = vec4(vTexCoord.xy, 0.0, 1.0);
}