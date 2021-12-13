#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

uniform float mouseRange;
uniform vec2 mousePos;

// the time value is passed into the shader by the OF app.
uniform float u_time;

vec4 displacement(vec4 mpos) {
    // the sine wave travels along the x-axis (across the screen),
    // so we use the x coordinate of each vertex for the calculation,
    // but we displace all the vertex along the y axis (up the screen)/
    float displacementHeight = 100.0;
    float displacementY = sin(u_time + (position.x / 100.0)) * displacementHeight;
    mpos.y += displacementY;
    
    return mpos;
}

vec4 peak(vec4 pos) {
    // direction vector from mouse position to vertex position.
    vec2 dir = pos.xy - mousePos;

    // distance between the mouse position and vertex position.
    float dist = distance(pos.xy, mousePos);

    // check vertex is within mouse range.
    if(dist > 0.0 && dist < mouseRange) {

        // normalise distance between 0 and 1.
        float distNorm = dist / mouseRange;

        // flip it so the closer we are the greater the repulsion.
        distNorm = 1.0 - distNorm;

        // make the direction vector magnitude fade out the further it gets from mouse position.
        dir *= distNorm;

        // add the direction vector to the vertex position.
        pos.x += dir.x;
        pos.y += dir.y;
    }

    return pos;
}

void main() {
    // copy position so we can work with it.
    vec4 pos = position;

    pos = peak(pos);
    vec4 mpos = modelViewProjectionMatrix * pos;
    mpos = displacement(mpos);
    gl_Position = mpos;
}