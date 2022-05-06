#version 150

#define N_USERS 10

// Color values
// r/x: lifeforce of cell, whats taken for gol logic instead of 0 and 1
// g/y: sum of lifeforce of neigbouring cells divided by 8 for clamping
// b/z: amount of influece, influence stronger if recently affected by interaction, WIP
uniform sampler2DRect cellData; // Previous generation cell data

uniform float evolutionFac;
uniform vec2 screen;
uniform vec2 resolution;
uniform float offset;

in vec2 vTexCoord;

out vec4 vFragColor;

void main(void){
    // Get the cell state 
    vec3 state = texture( cellData, vTexCoord ).xyz;
    vec4 next_state = vec4( state.xyz, 1.0 );
    vec3 evolVec = vec3 ( evolutionFac );
   
    // Get all neighbouring cells' information
    vec3 tl = texture(cellData, vTexCoord.xy + vec2(-1.0, -1.0)).xyz;
    vec3 tc = texture(cellData, vTexCoord.xy + vec2( 0.0, -1.0)).xyz;
    vec3 tr = texture(cellData, vTexCoord.xy + vec2( 1.0, -1.0)).xyz;
    vec3 cl = texture(cellData, vTexCoord.xy + vec2(-1.0,  0.0)).xyz;
    vec3 cr = texture(cellData, vTexCoord.xy + vec2( 1.0,  0.0)).xyz;
    vec3 bl = texture(cellData, vTexCoord.xy + vec2(-1.0,  1.0)).xyz;
    vec3 bc = texture(cellData, vTexCoord.xy + vec2( 0.0,  1.0)).xyz;
    vec3 br = texture(cellData, vTexCoord.xy + vec2( 1.0,  1.0)).xyz;

    float n_neighbours = tl.x + tc.x + tr.x + cl.x + cr.x + bl.x + bc.x + br.x;   
    next_state.y = n_neighbours / 8.0;

    if ( state.x <= 0.0) {
        // Dead with 2.5 to 3.5 is revived
        if( n_neighbours >= 2.5 && n_neighbours <= 3.5 ) {
            next_state.x += evolutionFac;
        }
    } else {
        // Alive with less than two dies
        if ( n_neighbours < 2.0 ) {
            next_state.x -= evolutionFac;

        // Alive with two to three lives
        } else if ( n_neighbours >= 2.0 && n_neighbours <= 3.0 ) {
            next_state.x += evolutionFac;

        // ALive with more than three dies
        } else if ( n_neighbours > 3.0 ) {
            next_state.x -= evolutionFac;
        }
    }

     // Clamp values between 0.0 and 1.0
    next_state.x = clamp(next_state.x, 0.0, 1.0);
    next_state.y = clamp(next_state.y, 0.0, 1.0);
    next_state.z = clamp(next_state.z, 0.0, 1.0);

    // And finally store it on the FBO
     vFragColor = next_state;
}