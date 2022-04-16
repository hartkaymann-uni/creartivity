#version 150

uniform sampler2DRect p; // pressure
uniform sampler2DRect w; // velocity

uniform vec2 gridSize;
uniform float gridScale;

out vec4 vFragColor;

void main() {
	vec2 coords = gl_FragCoord.xy; 
	vec2 uv = coords / gridSize.xy;

	vec2 xOffset = vec2(1.0, 0.0);
	vec2 yOffset = vec2(0.0, 1.0);

	float pl = texture(p, coords - xOffset).x;
	float pr = texture(p, coords + xOffset).x;
	float pb = texture(p, coords - yOffset).x;
	float pt = texture(p, coords + yOffset).x;

	float scale = 0.5 / gridScale;
	vec2 gradient = scale * vec2(pr - pl, pt - pb);

	vec2 wc = texture(w, coords).xy;

	vFragColor = vec4(wc - gradient, 0.0, 1.0);
}