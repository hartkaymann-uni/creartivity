#version 150

struct Cell
{
    bool alive;
    bool invincible;
    vec3 color;
};

uniform Cell u_Cells[102 * 77];

uniform mat4 u_MVP;

in vec4 position;

void main(){
    gl_Position = modelViewProjectionMatrix * position;
}