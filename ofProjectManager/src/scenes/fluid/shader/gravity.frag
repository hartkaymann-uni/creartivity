#version 150

uniform sampler2DRect velocity;

uniform vec2 dir;
uniform float str;

out vec4 vFragColor;

void main() {
	vec2 coords = gl_FragCoord.xy;
	vFragColor = vec4(texture(velocity, coords).xy + (normalize(dir) * str), 0.0, 1.0);
	//vFragColor = vec4(1.0, 0.0, 1.0, 1.0);
}