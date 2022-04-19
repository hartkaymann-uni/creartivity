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
vec2 bilerp(sampler2D tex, vec2 p) {
	vec4 ij; // i0, j0, i1, j1
    ij.xy = floor(p - 0.5) + 0.5;
    ij.zw = ij.xy + 1.0;

    vec4 uv = ij / gridSize.xyxy;
    vec2 d11 = texture(tex, uv.xy).xy;
    vec2 d21 = texture(tex, uv.zy).xy;
    vec2 d12 = texture(tex, uv.xw).xy;
    vec2 d22 = texture(tex, uv.zw).xy;

    vec2 a = p - ij.xy;
    
    return mix(mix(d11, d21, a.x), mix(d12, d22, a.x), a.y);
}

// Biliear Interpolation
// Source: https://github.com/dushyantbehl/2D-fluid-simulation/blob/master/pShader.cg
vec4 f4texRECTbilerp(sampler2DRect tex, vec2 s)
{
  vec4 st;
  st.xy = floor(s - 0.5) + 0.5;
  st.zw = st.xy + 1;
 
  vec2 t = s - st.xy; //interpolating factors
   
  vec4 tex11 = texture(tex, st.xy);
  vec4 tex21 = texture(tex, st.zy);
  vec4 tex12 = texture(tex, st.xw);
  vec4 tex22 = texture(tex, st.zw);

  // bilinear interpolation
  return mix(mix(tex11, tex21, t.x), mix(tex12, tex22, t.x), t.y);
}

void main() {
	vec2 uv = gl_FragCoord.xy / gridSize;
    float scale = 1.0 / gridScale;

    // trace points back in time
    vec2 p = uv - timestep * scale * texture(velocity, uv).xy;

    vFragColor = vec4((dissipation * bilerp(advected, p)), 0.0, 1.0);

    // Debug outs:
    //vFragColor = vec4(dissipation * bilerp(advected, p), 0.0, 1.0);
    //vFragColor = vec4(p, 0.0, 1.0);
    //vFragColor = texture(velocity, uv);
}