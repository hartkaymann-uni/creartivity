#version 150

uniform sampler2DRect cellData; // Previous generation cell data

uniform float timestep;
uniform float evolutionFac;
uniform vec2 screen;

in vec2 vTexCoord;

out vec4 vFragColor;

void main(void){
    // Get the cell state 
    vec3 state = texture( cellData, vTexCoord ).xyz;
    vec4 next_state = vec4( state.xyz, 0.1 );
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
            next_state.xyz += evolVec;
        }
    } else {
        // Alive with less than two dies
        if ( n_neighbours < 2.0 ) {
            next_state.xyz -= evolVec;

        // Alive with two to three lives
        } else if ( n_neighbours >= 2.0 && n_neighbours <= 3.0 ) {
            next_state.xyz += evolVec;

        // ALive with more than three dies
        } else if ( n_neighbours > 3.0 ) {
            next_state.xyz -= evolVec;
        }
    }

    // Clamp values between 0.0 and 1.0
    next_state.x = clamp(next_state.x, 0.0, 1.0);
    next_state.y = clamp(next_state.y, 0.0, 1.0);
    next_state.z = clamp(next_state.z, 0.0, 1.0);

    // And finally store it on the FBO
     vFragColor = vec4(next_state.xyz, 1.0);

     // Border
     int borderThickness = 0;
     if(vTexCoord.x <= borderThickness 
     || vTexCoord.x >= (screen.x - borderThickness) 
     || vTexCoord.y <= borderThickness 
     || vTexCoord.y >= (screen.y - borderThickness)) 
     {
        vFragColor = vec4( 1.0 );
     }
}