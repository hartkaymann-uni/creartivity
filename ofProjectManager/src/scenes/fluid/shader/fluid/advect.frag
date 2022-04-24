#version 150

uniform sampler2D velocity; 
uniform sampler2D advected; 

uniform vec2 gridSize;
uniform float gridScale;

uniform float timestep;
uniform float dissipation;

out vec4 vFragColor;

// Biliear Interpolation
// Source: https://github.com/mharrys/fluids-2d/blob/feb725984d2fe3f3830930e669afaea47d65d9a1/shaders/advect.fs
vec2 bilerp(sampler2D d, vec2 p)
{
    vec4 ij; // i0, j0, i1, j1
    ij.xy = floor(p - 0.5) + 0.5;
    ij.zw = ij.xy + 1.0;

    vec4 uv = ij / gridSize.xyxy;
    vec2 d11 = texture(d, uv.xy).xy;
    vec2 d21 = texture(d, uv.zy).xy;
    vec2 d12 = texture(d, uv.xw).xy;
    vec2 d22 = texture(d, uv.zw).xy;

    vec2 a = p - ij.xy;

    return mix(mix(d11, d21, a.x), mix(d12, d22, a.x), a.y);
}

// Biliear Interpolation
// Source: https://github.com/dushyantbehl/2D-fluid-simulation/blob/master/pShader.cg
vec3 f4texRECTbilerp(sampler2D tex, vec2 p)
{
  vec4 ij; // i0, j0, i1, j1
    ij.xy = floor(p - 0.5) + 0.5;
    ij.zw = ij.xy + 1.0;

    vec4 uv = ij / gridSize.xyxy;
    vec3 tex11 = texture(tex, uv.xy).xyz;
    vec3 tex21 = texture(tex, uv.zy).xyz;
    vec3 tex12 = texture(tex, uv.xw).xyz;
    vec3 tex22 = texture(tex, uv.zw).xyz;

    vec2 a = p - ij.xy;

    return mix(mix(tex11, tex21, a.x), mix(tex12, tex22, a.x), a.y);
}

void main() 
{
    vec2 uv = gl_FragCoord.xy / gridSize.xy;
    float scale = 1.0 / gridScale;

    // trace points back in time
    vec2 p = gl_FragCoord.xy - timestep * scale * texture(velocity, uv).xy;

    vFragColor = vec4(dissipation * f4texRECTbilerp(advected, p), 1.0);
    //vFragColor = vec4(dissipation * bilerp(advected, p), 0.0, 1.0);

    // Debug outs:
    //vFragColor = vec4(uv, 0.0, 1.0);
    //vFragColor = texture(velocity, uv);
}