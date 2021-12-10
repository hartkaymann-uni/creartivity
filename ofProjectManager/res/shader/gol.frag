#version 150

#define N_USERS 10

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

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void){
    // Get the cell state 
    vec3 state = texture( cellData, vTexCoord ).xyz;
    vec4 next_state = vec4( state.xyz, 1.0 );
    vec3 evolVec = vec3 ( evolutionFac );
   
    float left_top  = texture(cellData, vTexCoord.xy + vec2(-1.0, -1.0)).x;
    float mid_top   = texture(cellData, vTexCoord.xy + vec2( 0.0, -1.0)).x;
    float right_top = texture(cellData, vTexCoord.xy + vec2( 1.0, -1.0)).x;
    float left_mid  = texture(cellData, vTexCoord.xy + vec2(-1.0,  0.0)).x;
    float right_mid = texture(cellData, vTexCoord.xy + vec2( 1.0,  0.0)).x;
    float left_bot  = texture(cellData, vTexCoord.xy + vec2(-1.0,  1.0)).x;
    float mid_bot   = texture(cellData, vTexCoord.xy + vec2( 0.0,  1.0)).x;
    float right_bot = texture(cellData, vTexCoord.xy + vec2( 1.0,  1.0)).x;
    float n_neighbours = left_top + mid_top + right_top + left_mid + right_mid + left_bot + mid_bot + right_bot;
   
    if ( state.x <= 0.0) {
        // Dead with 2.5 to 3 is revived
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
    }

    // Hand interaction    
    for(int i = 0; i < N_USERS; i++) {
        if(distance(vTexCoord, hands[i].xy/offset) <= mouseRad) {
            next_state.x += mouseStr;
        }
    }

    next_state.y = n_neighbours / 8.0;

     // Clamp values between 0.0 and 1.0
    next_state.x = clamp(next_state.x, 0.0, 1.0);
    next_state.y = clamp(next_state.y, 0.0, 1.0);
    next_state.z = clamp(next_state.z, 0.0, 1.0);

    // And finally store it on the FBO
     vFragColor = next_state;
}