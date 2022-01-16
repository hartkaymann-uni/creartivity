#version 440

in vec4 position;
in vec4 color;

uniform float pointSize;
uniform mat4 modelViewProjectionMatrix;

out vec4 fragColor;



void main(){
    gl_Position = modelViewProjectionMatrix * position;

    fragColor = color;
    
    int maxDepth = 10000;
    float depth = abs(clamp(position.z, -maxDepth, 0));
    float sizeMod = (1-depth/maxDepth) * 1.5f + 0.5f;
    sizeMod = clamp(sizeMod, 0.5, 10.0);
    gl_PointSize = pointSize * sizeMod;

}