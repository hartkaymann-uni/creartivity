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

    vec2 xOffset = vec2(1.0 / gridSize.x, 0.0);
    vec2 yOffset = vec2(0.0, 1.0 / gridSize.y);

    float wl = texture( vorticity, uv - xOffset ).x;
    float wr = texture( vorticity, uv + xOffset ).x;
    float wb = texture( vorticity, uv - yOffset ).x;
    float wt = texture( vorticity, uv + yOffset ).x;
    float wc = texture( vorticity, uv ).x;

    float scale = 0.5 / gridScale;
    vec2 force = scale * vec2( abs( wt ) - abs( wb ), abs( wr ) - abs( wl ) );
    float lengthSquared = max( epsilon, dot( force, force ) );
    force *= inversesqrt( lengthSquared ) * curl * wc;
    force.y *= -1.0;

    vec2 velc = texture( velocity, uv ).xy;
    vFragColor  = vec4( velc + (timestep * force), 0.0, 1.0 );

    //vFragColor = vec4(0.0, 1.0, 0.0, 1.0);
}