#version 150

in vec2 vTexCoord;

uniform sampler2D read;

uniform vec2 gridSize;

uniform vec3 bias;
uniform vec3 scale;

out vec4 vFragColor; 

void main()
{
    vec3 c = vec3(0.f);
    float height = 1000;
    int y = int(vTexCoord.y * height);
    
    int offset = y % 10;
    
    vec2 st = vTexCoord;
    st.y = (y - offset) / height;
    //st.x = floor(st.x * 100.f) / 100.f;

    if(texture(read, st).x >= .2f) c = vec3(1.f);


    vFragColor = vec4(c, 1.0);
    //vFragColor = vec4(vTexCoord, vTexCoord);
}
