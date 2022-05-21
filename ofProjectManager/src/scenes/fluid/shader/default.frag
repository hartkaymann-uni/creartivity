#version 150

in vec2 vTexCoord;

uniform sampler2D read;

vec3 black = vec3(0.0);
vec3 white = vec3(1.0);
vec3 red = vec3(1.0, 0.0, 0.0);

out vec4 vFragColor;

void main()
{
    float density = texture(read, vTexCoord).x;

    vec3 col;
    col = mix(red, white, smoothstep(1.0, 0.5, density));
    col = mix(col, black, smoothstep(0.5, 0.0, density));

    vFragColor = vec4(col, 1.0);
}