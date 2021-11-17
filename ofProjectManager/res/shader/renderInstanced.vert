#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;
in vec4 color;
in vec4 normal;

uniform sampler2DRect cellTex;
uniform vec2 screen;
uniform vec2 resolution;
uniform float size;
uniform float radius;

out vec4 vPosition;
out vec4 vColor;
out vec3 vNormal;

void main() {
    
    vec2 coords = vec2( floor(gl_InstanceID / resolution.y), mod(gl_InstanceID, resolution.y));
    
    vec4 col = texture( cellTex, coords );
    vColor = vec4( vec3(col.x), 1.0 );

    float r = col.y * radius;
    vec4 pos = vec4( position.xyz * r, 1.0 ) ;
    vec4 offset = vec4( coords.xy * 2 * size, 0.0, 0.0);
    vPosition =  pos + offset;
    gl_Position = modelViewProjectionMatrix * vPosition;
    
    vNormal = normal.xyz;
}