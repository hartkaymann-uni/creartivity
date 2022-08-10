#version 440

in vec4 position;
in vec4 color;

uniform float pointSize;
uniform int max_particle_depth;
uniform mat4 modelViewProjectionMatrix;

out vec4 fragColor;



void main(){
    gl_Position = modelViewProjectionMatrix * position;

    fragColor = color;
    
    // Calculate the point size according to the uniform and particle depth
    float depth = abs(clamp(position.z, -max_particle_depth, 0));
    float sizeMod = (1-depth/max_particle_depth) * 1.5f + 0.5f;
    sizeMod = clamp(sizeMod, 0.5, 10.0);
    gl_PointSize = pointSize * sizeMod;

}