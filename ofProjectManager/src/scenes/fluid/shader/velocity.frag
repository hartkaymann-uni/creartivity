#version 150

vec3 black = vec3(0.0);
vec3 white = vec3(1.0);
vec3 red = vec3(1.0, 0.0, 0.0);

uniform sampler2D read;
uniform sampler2D velocity;

in vec2 vTexCoord;

out vec4 vFragColor; 


void main()
{
    float base = texture(read, vTexCoord).x;

    vec2 vel = texture(velocity, vTexCoord).xy;
    float speed = dot(vel, vel);

    float white = smoothstep(1.5, 1.0, speed);
    vec3 col = vec3(1.0, white, white);

    vFragColor = vec4(base * col, 1.0);
   // vFragColor = vec4(vTexCoord, vTexCoord);
}
