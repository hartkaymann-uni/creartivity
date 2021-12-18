#version 150
uniform float u_time;
uniform float u_amplitude;
uniform float u_thickness;
uniform vec2 u_mouse;

in vec4 vPosition;
in vec4 vColor;

out vec4 vFragColor;

void main() {
	int colorDist = 100;
		
	vec4 c = vec4(1.0 - round(mod(vPosition.z * u_thickness, u_thickness * 2.0)));
	if(vColor.x > 0.0) {
		c.yz -= vec2( vColor.x);
	}

	if(vPosition.z >= u_amplitude) c = vec4(0.0);
	 
	if(vPosition.z <= 1.5 && vPosition.z >= -1.5) c = vec4(0.0);
	//if(vPosition.z < 1.99 && distance(vPosition.xy, u_mouse.xy) < colorDist) c = vec4(1.0-distance(vPosition.xy, u_mouse.xy)/colorDist,0.0,0.0,1.0);
	if(vPosition.z <= -2.001) c = vec4(0.0);
	
	float r = abs(vPosition.z / 5);

	vFragColor = vec4(c);
}