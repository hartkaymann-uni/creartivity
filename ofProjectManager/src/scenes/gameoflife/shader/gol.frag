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

uniform bool mouseDown;
uniform vec3 mousePos;
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
    vec3 left_top  = texture(cellData, vTexCoord.xy + vec2(-1.0, -1.0)).xyz;
    vec3 mid_top   = texture(cellData, vTexCoord.xy + vec2( 0.0, -1.0)).xyz;
    vec3 right_top = texture(cellData, vTexCoord.xy + vec2( 1.0, -1.0)).xyz;
    vec3 left_mid  = texture(cellData, vTexCoord.xy + vec2(-1.0,  0.0)).xyz;
    vec3 right_mid = texture(cellData, vTexCoord.xy + vec2( 1.0,  0.0)).xyz;
    vec3 left_bot  = texture(cellData, vTexCoord.xy + vec2(-1.0,  1.0)).xyz;
    vec3 mid_bot   = texture(cellData, vTexCoord.xy + vec2( 0.0,  1.0)).xyz;
    vec3 right_bot = texture(cellData, vTexCoord.xy + vec2( 1.0,  1.0)).xyz;

    float n_neighbours = left_top.x + mid_top.x + right_top.x + left_mid.x + right_mid.x + left_bot.x + mid_bot.x + right_bot.x;   
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

    // Mouse interaction
    if(mouseDown && distance(vTexCoord, mousePos.xy / offset) <= mouseRad) {
        next_state.x += mouseStr;
        next_state.z = 1.0;
    }

    // Hand interaction    
    for(int i = 0; i < N_USERS; i++) {
        if(hands[i].xy != vec2(0.0)){
            if(distance(vTexCoord, hands[i].xy/offset) <= mouseRad) {
                next_state.x += mouseStr;
                next_state.z = 1.0;
            }
        } 
     }
    float influence = max(left_top.z, max(mid_top.z, max( right_top.z, max( left_mid.z, max( right_mid.z, max( left_bot.z, max( mid_bot.z, max( right_bot.z, state.z ))))))));

    if(state.x > 0) {
        next_state.z = influence * (1.0 - evolutionFac);

        if(next_state.z > 0.0) {
        influence -= evolutionFac;    
    }
}
    

    // average influence of neighbouring cells        
    

     // Clamp values between 0.0 and 1.0
    next_state.x = clamp(next_state.x, 0.0, 1.0);
    next_state.y = clamp(next_state.y, 0.0, 1.0);
    next_state.z = clamp(next_state.z, 0.0, 1.0);

    // And finally store it on the FBO
     vFragColor = next_state;
}