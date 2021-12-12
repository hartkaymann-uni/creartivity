#version 150

in vec4 fragColor;

uniform vec2 particlePosition;

out vec4 outputColor;


void main()
{
    vec2 fragCoord = gl_FragCoord.xy;
    fragCoord.y = fragCoord.y;
    float dist = distance(fragCoord, particlePosition);
    dist = clamp(dist, 0, 50);
    float r, g = 0.0f;
    float b = 0.02f * dist;
    outputColor = vec4(r,g,b, 1.0f);
}