#version 440

//precision mediump float;

in vec4 fragColor;

uniform float alphaMod;

out vec4 outputColor;

void main() {
    float alpha = clamp(fragColor.a * alphaMod, 0.05, 1.0);
    vec4 colorOut = fragColor;
    colorOut.a = alpha;
    outputColor = colorOut;
}