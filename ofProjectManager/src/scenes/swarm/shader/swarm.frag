#version 440

in vec4 fragColor;

uniform float alphaMod;

out vec4 outputColor;

void main() {
    // Changes the calculated particle transparency according to alphaMod
    float alpha = clamp(fragColor.a * alphaMod, 0.05, 1.0);
    vec4 colorOut = fragColor;
    colorOut.a = alpha;
    
    outputColor = colorOut;
}