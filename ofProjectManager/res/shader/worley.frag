#version 150

#ifdef GL_ES
precision mediump float;
#endif

in vec2 vTexCoord;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

uniform int u_length;
uniform vec2 u_positions[88];
uniform vec2 u_position;

out vec4 vFragColor;

vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    vec3 color = vec3(.0);

    // Scale
    st *= 10.;

    // Tile the space
    vec2 i_st = floor(st);
    vec2 f_st = fract(st);

    float m_dist = 1.;  // minimum distance

    for (int y= -1; y <= 1; y++) {
        for (int x= -1; x <= 1; x++) {
            // Neighbor place in the grid
            vec2 neighbor = vec2(float(x),float(y));

            // Random position from current + neighbor place in the grid
            vec2 point = random2(i_st + neighbor);

			// Animate the point
            point = 0.5 + 0.4*sin(0.3*u_time + 6.2831*point);

			// Vector between the pixel and the point
            vec2 diff = neighbor + point - f_st;

            // Distance to the point
            float dist = length(diff);

            // Keep the closer distance
            m_dist = min(m_dist, dist);
        }
    }

    // Draw the min distance (distance field)
    color += m_dist;

    // Draw cell center
    //color += 1.-step(.02, m_dist);

    // Draw grid
    //color.r += step(.98, f_st.x) + step(.98, f_st.y);

    // Show isolines
    // color -= step(.7,abs(sin(27.0*m_dist)))*.5;

    float minDist = 1.0;
    for( int i = 0; i < 81; i++) {
    vec2 pos = u_positions[i];
    float dist = distance(pos.xy / u_resolution.xy, gl_FragCoord.xy / u_resolution.xy);
        minDist = dist < minDist ? dist : minDist;

    }

    vFragColor = vec4(vec3(smoothstep(0.0, 0.5, minDist)), 1.0);

}
