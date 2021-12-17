#version 440

in vec4 position;
in vec4 color;

uniform float pointSize;
uniform mat4 modelViewProjectionMatrix;

out vec4 fragColor;



void main(){
    gl_Position = modelViewProjectionMatrix * position;

    fragColor = color;
    
    // float depth = clamp(position.z, -500, 500);
    // float sizeMod = (depth + 500)/1000 * 2.0f;
    // gl_PointSize = pointSize * sizeMod;
}