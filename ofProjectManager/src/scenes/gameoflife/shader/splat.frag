#version 150

in vec2 vTexCoord;

uniform sampler2DRect cellData;

uniform vec2 resolution;
uniform vec2 point;

uniform float evolutionFac;
uniform float mouseStr;
uniform float mouseRad;

out vec4 vFragColor;

float gauss(vec2 p, float r) 
{
	return exp(-dot(p, p) / r);
}

void main() 
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    uv.y = 1 - uv.y;

    vec3 base = texture( cellData, vTexCoord).xyz;
    vec3 splat = vec3(0.f);

    // Get all neighbouring cells' information
    float tl = texture(cellData, vTexCoord.xy + vec2(-1.0, -1.0)).z;
    float tc = texture(cellData, vTexCoord.xy + vec2( 0.0, -1.0)).z;
    float tr = texture(cellData, vTexCoord.xy + vec2( 1.0, -1.0)).z;
    float cl = texture(cellData, vTexCoord.xy + vec2(-1.0,  0.0)).z;
    float cr = texture(cellData, vTexCoord.xy + vec2( 1.0,  0.0)).z;
    float bl = texture(cellData, vTexCoord.xy + vec2(-1.0,  1.0)).z;
    float bc = texture(cellData, vTexCoord.xy + vec2( 0.0,  1.0)).z;
    float br = texture(cellData, vTexCoord.xy + vec2( 1.0,  1.0)).z;
    
    vec2 p = point - uv;
    p.x *= resolution.x / resolution.y;

    if(length(p) < mouseRad) {
        splat.x = mouseStr;
        splat.z = 1.f;
    }

    // Find the highest interaction factor in all neighbouring cells
    float influence = max(tl, max(tc, max( tr, max( cl, max( cr, max( bl, max( bc, max( br, base.z ))))))));

    // Set interaction factor of a cell by indirect interaction only once
    if(base.x > 0 && base.z <= 0) {
        splat.z = influence * (0.9f);
    }

    splat.z -= 0.1 * evolutionFac;     
        
	vFragColor = vec4(base + splat, 1.f);
	
    // Debug
    //vFragColor = vec4(uv, uv);
}