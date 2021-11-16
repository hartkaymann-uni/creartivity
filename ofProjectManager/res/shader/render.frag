#version 150

uniform sampler2DRect cellTex;

uniform vec2 screen;
uniform float size;

flat in vec4 vPosition;
flat in vec2 vTexCoord;
flat in vec4 vColor;

out vec4 vFragColor;

void main() {

	vFragColor = vec4(0.0);
	
	mat4x2 coords;
	coords[0] = vTexCoord.xy;
	coords[1] = vTexCoord.xy + vec2( 1.0,  0.0 ); 
	coords[2] = vTexCoord.xy + vec2( 0.0, -1.0 );
	coords[3] = vTexCoord.xy + vec2( 1.0, -1.0 );

	mat4 states;
	states[0] = texture( cellTex, coords[0] );
	states[1] = texture( cellTex, coords[1] );
	states[2] = texture( cellTex, coords[2] );
	states[3] = texture( cellTex, coords[3] );

	float maxState = 0.0;
	for(int i = 0; i < 4; i++) {
		vec3 state = states[i].xyz;
		float r = states[i].w * size;

		float dist = distance( coords[i], gl_FragCoord.xy / size);
		if( state.x > dist ) {
			vFragColor = vec4( state.xyz, 1.0 );
		}
		maxState = state.x > maxState ? state.x : maxState;
	}

//	vFragColor = vec4( vec3( distance(coords[3], gl_FragCoord.xy / size )), 1.0 );
//	vFragColor = texture( cellTex, vTexCoord.xy + vec2( 0.0, -1.0) );
//	vFragColor = vec4( vec3( maxState ), 1.0 );
}