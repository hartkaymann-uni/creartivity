#version 150

out vec4 outputColor;

void main()
{
    float r = gl_FragCoord.x/1000;
    float g = 0.0f;
    float b = gl_FragCoord.y/1000;
    float a = 1.0f;

    outputColor = vec4(r, g, b, a);
}