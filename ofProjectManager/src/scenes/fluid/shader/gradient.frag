#version 150

in vec2 vTexCoord;

uniform sampler2DRect p; // pressure
uniform sampler2DRect w; // velocity

uniform vec2 gridSize;
uniform float gridScale;

out vec4 vFragColor;

void main() {
	vec2 uv = vTexCoord.xy / gridSize.xy;

	vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
	vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

	float pr = texture(p, uv + xOffset).x;
	float pt = texture(p, uv + yOffset).x;
	float pl = texture(p, uv - xOffset).x;
	float pb = texture(p, uv - yOffset).x;

	float scale = 0.5 / gridScale;
	vec2 gradient = scale * vec2(pr - pl, pt - pb);

	vec2 wc = texture(w, uv).xy;

	vFragColor = vec4(wc - gradient, 0.0, 1.0);
}