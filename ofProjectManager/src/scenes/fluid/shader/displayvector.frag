#version 150

in vec2 vTexCoord;

uniform sampler2DRect read;

uniform vec3 bias;
uniform vec3 scale;

uniform vec2 gridSize;

out vec4 vFragColor; 

void main()
{
	vec2 uv = vTexCoord / gridSize;

    vFragColor = vec4(bias + scale * texture(read,  vTexCoord).xyz, 1.0);
	//vFragColor = vec4(vTexCoord / gridSize, 0.0, 1.0);
}