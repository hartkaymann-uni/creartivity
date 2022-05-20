#version 150

uniform sampler2D read;

uniform vec2 gridSize;

uniform float threshhold;

in vec2 vTexCoord;

out vec4 vFragColor;

void main() 
{
	vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
    vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

	vec4 sum = vec4(0.0);
	sum += texture(read, vTexCoord  + xOffset);
	sum += texture(read, vTexCoord  + yOffset);
	sum += texture(read, vTexCoord  - xOffset);
	sum += texture(read, vTexCoord  - yOffset);

    vFragColor = sum;
	//vFragColor = texture(read, vTexCoord + xOffset);
}
