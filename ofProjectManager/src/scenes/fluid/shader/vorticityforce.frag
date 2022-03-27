#version 150

uniform sampler2D velocity;
uniform sampler2D vorticity;

uniform vec2 gridSize;
uniform float gridScale;

uniform float timestep;
uniform float epsilon;
uniform vec2 curl;

out vec4 vFragColor;

void main()
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;

    vec2 xOffset = vec2( 1.0 / gridSize.x, 0.0 );
    vec2 yOffset = vec2( 0.0, 1.0 / gridSize.y );

    float vl = texture( vorticity, uv - xOffset ).x;
    float vr = texture( vorticity, uv + xOffset ).x;
    float vb = texture( vorticity, uv - yOffset ).x;
    float vt = texture( vorticity, uv + yOffset ).x;
    float vc = texture( vorticity, uv ).x;

    float scale = 0.5 / gridScale;
    vec2 force = scale * vec2( abs( vt ) - abs( vb ), abs( vr ) - abs( vl ) );
    float lengthSquared = max( epsilon, dot( force, force ) );
    force *= inversesqrt( lengthSquared ) * curl * vc;
    force.y *= -1.0;

    vec2 velc = texture( velocity, uv ).xy;
    vFragColor  = vec4( velc + (timestep * force), 0.0, 1.0 );
}