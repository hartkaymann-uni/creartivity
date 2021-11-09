#version 150

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 6) out;

uniform mat4 modelViewProjectionMatrix;

uniform float size;

in vec4 vPosition[];

out vec2 vTexCoord;

void main(void) {
    for(int i = 0; i < gl_in.length(); i++) {
		gl_Position = modelViewProjectionMatrix * ( vPosition[i] + vec4(0.0, -size, 0.0, 0.0));
		vTexCoord = vPosition[i].xy;
		EmitVertex();

		gl_Position = modelViewProjectionMatrix * ( vPosition[i] + vec4(size, -size, 0.0, 0.0));
		vTexCoord = vPosition[i].xy;
		EmitVertex();

		gl_Position = modelViewProjectionMatrix * ( vPosition[i] + vec4(size, 0.0, 0.0, 0.0));
		vTexCoord = vPosition[i].xy;
		EmitVertex();

		gl_Position = modelViewProjectionMatrix * ( vPosition[i] + vec4(0.0, -size, 0.0, 0.0));
		vTexCoord = vPosition[i].xy;
		EmitVertex();

		gl_Position = modelViewProjectionMatrix * ( vPosition[i] + vec4(0.0, 0.0, 0.0, 0.0));
		vTexCoord = vPosition[i].xy;
		EmitVertex();

		gl_Position = modelViewProjectionMatrix * ( vPosition[i] + vec4(size, 0.0, 0.0, 0.0));
		vTexCoord = vPosition[i].xy;
		EmitVertex();
		EndPrimitive();
	}
}