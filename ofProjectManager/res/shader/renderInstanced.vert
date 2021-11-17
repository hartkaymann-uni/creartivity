#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
in vec4 color;

uniform sampler2DRect cellTex;
uniform vec2 screen;
uniform vec2 resolution;
uniform float size;


flat out vec4 vColor;

void main() {
    
    vec2 coords = vec2( floor(gl_InstanceID / resolution.y), mod(gl_InstanceID, resolution.y));
    
    vec4 col = texture( cellTex, coords );
    vColor = vec4( vec3(col.x), 1.0 );

    float r = col.y * 8.0;
    vec3 pos = position.xyz * r;
    
    vec4 vPos = vec4( pos, 1.0 ) + vec4( coords.xy * 2 * size, 0.0, 0.0);
    
    gl_Position = modelViewProjectionMatrix * vPos;
}