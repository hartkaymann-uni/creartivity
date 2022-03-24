#version 150

in vec2 vTexCoord;

uniform sampler2DRect u; // velocity grid
uniform sampler2DRect x; // field to be advected

uniform float timestep;
uniform float rdx;

out vec4 vFragColor;

// Biliear Interpolation of rectangular texture
// Source: https://community.khronos.org/t/bilinear-interpolation-texture-rendering/52613/2
vec4 texRectBilinear(sampler2DRect tex, vec2 uv) {
	vec2 weight = fract(uv);
	
	vec4 bottom = mix(texture(tex, uv), 
		texture(tex, uv + vec2(1, 0)), 
		weight.x);

	vec4 top = mix(texture(tex, uv + vec2(0, 1)), 
		texture(tex, uv + vec2(1, 1)), 
		weight.x);

	return mix(bottom, top, weight.y);

}

void main() {
	// follow velocity "back in time"
	vec2 pos = vTexCoord - timestep * rdx * texture(u, vTexCoord).xy;
	
	// interpolate and write to output
	vFragColor = texRectBilinear(x, pos);
}