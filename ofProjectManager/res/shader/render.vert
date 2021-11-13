#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
in vec4 color;

uniform sampler2DRect cellTex;
uniform vec2 screen;

out vec4 vPosition;
out vec2 vTexCoord;
out vec4 vColor;

void main() {
    // Read position data from texture.
    //vec4 pixPos = texture( cellTex, texcoord );
    
    // Map the position from the texture (from 0.0 to 1.0) to
    // the screen position (0 - screenWidth/screenHeight)
    //pixPos.x *= screen.x;
    //pixPos.y *= screen.y;

    vTexCoord = texcoord;
    vColor = vec4(0.5, 0.0, 0.0, 1.0);
}