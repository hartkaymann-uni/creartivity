#version 150

in vec2 vTexCoord;

uniform sampler2DRect read;

uniform vec3 bias;
uniform vec3 scale;

uniform vec2 gridSize;

out vec4 vFragColor; 

void main()
{
    vec2 coords = gl_FragCoord.xy;
    vec2 uv = gl_FragCoord.xy / gridSize.xy;

    vFragColor = vec4(texture(read, vTexCoord * gridSize).xyz, 1.0);
    vFragColor = vec4(uv, 0.0, 1.0);
}