#version 150

uniform sampler2DRect cellData; // Previous generation cell data

uniform float timestep;
uniform int resolutionX;
uniform int resolutionY;

in vec2 vTexCoord;

out vec4 vFragColor;

void main(void){
    // Get the cell state 
    vec3 state = texture( cellData, vTexCoord ).xyz;
    vec4 next_state = vec4( 0.0, 0.0, 0.0, 1.0 );
   
    float left_top  = floor( texture(cellData, vTexCoord.xy + vec2(-1.0, -1.0)).x + 0.5 );
    float mid_top   = floor( texture(cellData, vTexCoord.xy + vec2( 0.0, -1.0)).x + 0.5 );
    float right_top = floor( texture(cellData, vTexCoord.xy + vec2( 1.0, -1.0)).x + 0.5 );
    float left_mid  = floor( texture(cellData, vTexCoord.xy + vec2(-1.0,  0.0)).x + 0.5 );
    float right_mid = floor( texture(cellData, vTexCoord.xy + vec2( 1.0,  0.0)).x + 0.5 );
    float left_bot  = floor( texture(cellData, vTexCoord.xy + vec2(-1.0,  1.0)).x + 0.5 );
    float mid_bot   = floor( texture(cellData, vTexCoord.xy + vec2( 0.0,  1.0)).x + 0.5 );
    float right_bot = floor( texture(cellData, vTexCoord.xy + vec2( 1.0,  1.0)).x + 0.5 );
    float n_neighbours = left_top + mid_top + right_top + left_mid + right_mid + left_bot + mid_bot + right_bot;
   
    if (n_neighbours == 3.0) next_state.xyz = vec3( 1.0 ); 
    else if (state.x >= 0.5 && n_neighbours == 2.0) next_state.xyz = vec3( 1.0 );
        
    // And finally store it on the FBO
     vFragColor = vec4(next_state.xyz, 1.0);

     // Border
     int borderThickness = 1;
     if(vTexCoord.x <= borderThickness 
     || vTexCoord.x >= (resolutionX - borderThickness) 
     || vTexCoord.y <= borderThickness 
     || vTexCoord.y >= (resolutionY - borderThickness)) 
     {
        vFragColor = vec4( 1.0 );
     }
}