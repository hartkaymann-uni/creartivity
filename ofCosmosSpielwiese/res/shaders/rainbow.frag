#version 150

out vec4 outputColor;
uniform vec4 globalColor;
uniform float u_time;

void main() {
    outputColor = globalColor;
}

void windowGradient(){
    float windowWidth = 1024.0;
    float windowHeight = 768.0;

    float r = gl_FragCoord.x / windowWidth;
    float g = gl_FragCoord.y / windowHeight;
    float b = 1.0f;
    float a = 1.0f;

    outputColor = vec4(r, g, b, a);
}