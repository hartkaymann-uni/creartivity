#version 150

/**
struct RGB
{
    unsigned char 
    r, 
    g, 
    b;
};

struct Cell
{
    bool alive;
    RGB color;
};

uniform Cell cells[102 * 77];
*/
uniform mat4 modelViewProjectionMatrix;

in vec4 position;

void main(){
    gl_Position = modelViewProjectionMatrix * position;
}