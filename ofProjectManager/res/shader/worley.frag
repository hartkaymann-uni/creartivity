#version 150

#ifdef GL_ES
precision mediump float;
#endif

in vec2 vTexCoord;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

uniform int u_length;
uniform vec2 u_positions[88];
uniform vec2 u_position;

out vec4 vFragColor;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

void main() {

    float minDist = 1.0;
    for( int i = 0; i < 81; i++) {
    vec2 pos = u_positions[i];
    float dist = distance(pos.xy / u_resolution.xy, gl_FragCoord.xy / u_resolution.xy);
        minDist = dist < minDist ? dist : minDist;

    }
    vec3 color = vec3(smoothstep(0.0, 0.2, minDist));
    color += vec3(1.-step(.002, minDist));

    vFragColor = vec4(color, 1.0);
    // vFragColor = vec4(.5, 0., 0., 1.0);
}
