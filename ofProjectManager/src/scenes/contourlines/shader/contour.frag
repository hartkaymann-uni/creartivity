#version 150

uniform float u_time;
uniform float u_amplitude;
uniform float u_thickness;
uniform vec2 u_mouse;

uniform vec3 viewPos;
float specularStrength = 0.5;

vec3 lightDir = vec3(-1.f, 1.f, 1.f);
vec3 lightCol = vec3(1.f, 1.f, 1.f);

float ambientStrength = 0.5f;
vec3 ambientColor = vec3(1.f, 1.f, 1.1f);

in vec4 vPosition;
in vec3 vNormal;
in vec3 vColor;

out vec4 vFragColor;

void main() {
	// Ambient lighting
	vec3 ambient = ambientStrength * ambientColor;
	
	// Diffuse Lighting
	vec3 ld = vec3(cos(u_time), sin(u_time), lightDir.z);
	float diff = max(dot(vNormal, normalize(ld)), 0.0);
	vec3 diffuse = diff * lightCol; 

	vec3 col = vec3(sin(vPosition.z));

	vFragColor = vec4( (ambient + diffuse) * col, 1.f);
	//vFragColor = vec4( vNormal, 1.f);
}