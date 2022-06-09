#version 150

in vec4 vPosition;

uniform sampler2DRect cells;
uniform vec2 screen;
uniform vec2 resolution;
uniform float radius;

out vec4 vFragColor;

vec2 rand( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

void main() 
{
	vec2 uv = gl_FragCoord.xy / screen.xy;
	vec2 st = uv * resolution.xy;
	vec4 cell =  texture(cells, st);
    vec3 color = vec3(1.f);
    float interaction = 0.f;

	vec2 i_st = floor(st) + vec2(0.5);
	vec2 f_st = fract(st);
    
    float m_dist = 1.f;
    int scanrad = 2;
    for (int j= -scanrad; j <= scanrad; j++ ) {
        for (int i= -scanrad; i <= scanrad; i++ ) {
            // First get offset and radius of neighbour
            vec2 neighbour = vec2(float(i), float(j));
            vec3 curr = texture(cells, i_st + neighbour).xyz;
            float rad = curr.x;

            // Add up interaction factor to get average later
            interaction += curr.z;

            if(rad > 0) {
                // Distance to neighbour minus its radius
                float dist = length( f_st - neighbour) - (rad);

                // Metaball it!
                m_dist = min(m_dist, m_dist*dist);
            }
        }
    }
    // Draw cells
	//m_dist = length(i_st - st);
    color -= step(radius * .01, m_dist);
    color -= 1.f - step(radius * .0025, m_dist);

    // Get average of inteaction factor and adjust color to it
    interaction /=  pow(2 * scanrad + 1, 2);
    color -= vec3(0.f, interaction, interaction);

	vFragColor = vec4(color, 1.f);
	//vFragColor = vec4(st, 0.0, 1.0);
}