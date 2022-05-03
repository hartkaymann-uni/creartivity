#version 150

#define N_USERS 10

// Color values
// r/x: lifeforce of cell, whats taken for gol logic instead of 0 and 1
// g/y: sum of lifeforce of neigbouring cells divided by 8 for clamping
// b/z: amount of influece, influence stronger if recently affected by interaction, WIP
uniform sampler2DRect cellData; // Previous generation cell data

uniform float evolutionFac;
uniform vec2 screen;
uniform float offset;

uniform float mouseRad;
uniform float mouseStr;
uniform vec2 hands[N_USERS];

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
   
    // Interaction    
    for(int i = 0; i < N_USERS; i++) {
        if(hands[i].xy != vec2(0.0)){
            if(distance(vTexCoord, hands[i].xy/offset) <= mouseRad) {
                next_state.x += mouseStr;
                next_state.z = 1.0;
            }
        } 
     }

     // Find the highest interaction factor in all neighbouring cells
    float influence = max(tl.z, max(tc.z, max( tr.z, max( cl.z, max( cr.z, max( bl.z, max( bc.z, max( br.z, state.z ))))))));

    // Set interaction factor of a cell by indirect interaction only once
    if(next_state.x > 0 && next_state.z <= 0) {
        next_state.z = influence * (0.9 - evolutionFac);
    } else if (state.z > 0.0) {
        next_state.z = state.z - evolutionFac;    
    }
        

     // Clamp values between 0.0 and 1.0
    next_state.x = clamp(next_state.x, 0.0, 1.0);
    next_state.y = clamp(next_state.y, 0.0, 1.0);
    next_state.z = clamp(next_state.z, 0.0, 1.0);

    // And finally store it on the FBO
     vFragColor = next_state;
}