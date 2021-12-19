#version 440

in vec4 position;
in vec4 color;

uniform float pointSize;
uniform mat4 modelViewProjectionMatrix;

out vec4 fragColor;



void main(){
    gl_Position = modelViewProjectionMatrix * position;

    fragColor = color;
    
    float depth = clamp(position.z, -10000, 0) + 10000;
    float sizeMod = (depth)/10000 * 2.0f;
    sizeMod = clamp(sizeMod, 0.5, 1.5);
    gl_PointSize = pointSize * sizeMod;

}