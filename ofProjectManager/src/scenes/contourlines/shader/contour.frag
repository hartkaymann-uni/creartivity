#version 150

uniform float u_time;
uniform float u_amplitude;
uniform float u_thickness;

//uniform bool u_moving;

uniform vec2 u_mouse;
uniform vec3 viewPos;

//float specularStrength = 0.5;

//vec3 lightDir = vec3(-1.f, 1.f, 1.f);
//vec3 lightCol = vec3(1.f, 1.f, 1.f);

//float ambientStrength = 0.5f;
//vec3 ambientColor = vec3(1.f, 1.f, 1.1f);

in vec4 vPosition;
//in vec3 vNormal;
in vec3 vColor;

out vec4 vFragColor;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {
	// Ambient lighting
	//vec3 ambient = ambientStrength * ambientColor;
	
	// Diffuse Lighting
	//vec3 ld = lightDir;
	//if( u_moving ) ld = vec3( cos( u_time ), sin( u_time ), lightDir.z );
		
	//float diff = max( dot( vNormal, normalize(ld) ), 0.0 );
	//vec3 diffuse = diff * lightCol; 

	//vec3 col = vec3( sin( vPosition.z * u_thickness) );
	//col = vec3( map( vPosition.z, -u_amplitude, u_amplitude, 0.0, 1.0 ) );
	vec3 col = vec3( step(fract(vPosition.z), u_thickness)) * vColor;

//	float f = fract(vPosition.z * u_thickness);
//	float df = fwidth(0.1 * f * u_thickness);
//	float g = smoothstep(df * 1.0, df * 2.0, f);
//	vec3 col = vec3(1 - g);

	//if(vPosition.z > u_amplitude * 0.66) col.yz = vec2(0.0);

	//vFragColor = vec4( ( ambient + diffuse ) * col, 1.f);
	vFragColor = vec4( col, 1.0);
	//vFragColor = vec4( vPosition.z > 0);
}