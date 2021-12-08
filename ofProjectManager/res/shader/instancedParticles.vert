#version 440

struct Particle{
	vec4 pos;
	vec4 vel;
	vec4 color;
};

layout(std140, binding=0) buffer particle{
    Particle p[];
};

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor = vec4(1.0);

in vec4  position;
in vec2  texcoord;
in vec4  color_coord;
in vec3  normal;

void main()
{
 vec4 vPos = p[gl_InstanceID].pos + position;
 gl_Position =  modelViewProjectionMatrix * vPos;
}