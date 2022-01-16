#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

uniform vec2 grid;
uniform vec3 cube;

out vec4 vPosition;
out vec2 vTexCoord;

void main() {
    vec2 coords = vec2( floor(gl_InstanceID / grid.y), mod(gl_InstanceID, grid.y));
    
    vec4 pos = position;
    pos.x += (1.2 * cube.x ) * coords.x;
    pos.y += (1.1 * cube.y ) * coords.y;

    vPosition =  pos;

    gl_Position = modelViewProjectionMatrix * vPosition;
    
   
}
