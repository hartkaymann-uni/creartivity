#version 440

in vec4 fragColor;

uniform float radius;
uniform vec3 hand_position;
uniform vec2 screen_resolution;

out vec4 outputColor;

void main() {
    vec4 colorOut = fragColor;
    
    vec2 invertedHandPosition = vec2(hand_position.x, screen_resolution.y - hand_position.y);
    float distance = distance(gl_FragCoord.xy, invertedHandPosition.xy);

    float percentage = 1 - (max(min(distance / radius, 1.0), 0.0));
    //float percentage = distance / 500;

    colorOut = vec4(0.5, 0, 0, 1);

    colorOut.a = 0.65 * pow(percentage,2); 
    

    outputColor = colorOut;
    
}