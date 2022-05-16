#version 150

in vec2 vTexCoord;

uniform sampler2D read;

uniform vec2 gridSize;

uniform vec3 bias;
uniform vec3 scale;

out vec4 vFragColor; 

void main()
{
    vec3 c = vec3(0.0);
    float height = 1000;
    int y = int(vTexCoord.y * height);
    
    int offset = y % 10;
    
    vec2 st = vTexCoord;
    st.y = (y - offset) / height;
    //st.x = floor(st.x * 100.f) / 100.f;

    vec3 tex = texture(read, st).xyz; 
    if(tex.x >= .2f) {
        if(tex.x - tex.y > 0.5)
          c = vec3(1.0, 0.0, 0.0);
        else
          c = vec3(1.0);
    }

    vFragColor = vec4(c, 1.0);
    //vFragColor = vec4(vTexCoord, vTexCoord);
}
