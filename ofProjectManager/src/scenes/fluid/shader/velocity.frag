#version 150

in vec2 vTexCoord;

uniform sampler2D read;

uniform vec2 gridSize;

uniform vec3 bias;
uniform vec3 scale;

out vec4 vFragColor; 

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    
    vec2 tex = texture(read, vTexCoord).xy; 
    float mag = length(tex);

    vec3 col = vec3(mag);   

    vFragColor = vec4(col, 1.0);
    //vFragColor = vec4(vTexCoord, vTexCoord);
}
