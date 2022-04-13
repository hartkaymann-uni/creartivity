#version 150

uniform sampler2DRect velocity;
uniform sampler2DRect vorticity;

uniform vec2 gridSize;
uniform float gridScale;

uniform float timestep;
uniform float epsilon;
uniform vec2 curl;

out vec4 vFragColor;

void main()
{
    vec2 coords = gl_FragCoord.xy;

    vec2 xOffset = vec2( 1.0, 0.0 );
    vec2 yOffset = vec2( 0.0, 1.0);

    float wl = texture( vorticity, coords - xOffset ).x;
    float wr = texture( vorticity, coords + xOffset ).x;
    float wb = texture( vorticity, coords - yOffset ).x;
    float wt = texture( vorticity, coords + yOffset ).x;
    float wc = texture( vorticity, coords ).x;

    float scale = 0.5 / gridScale;
    vec2 force = scale * vec2( abs( wt ) - abs( wb ), abs( wr ) - abs( wl ) );
    float lengthSquared = max( epsilon, dot( force, force ) );
    force *= inversesqrt( lengthSquared ) * curl * wc;
    force.y *= -1.0;

    vec2 velc = texture( velocity, coords ).xy;
    vFragColor  = vec4( velc + (timestep * force), 0.0, 1.0 );

    //vFragColor = vec4(0.0, 1.0, 0.0, 1.0);
}