#include "particle.h"

particle::particle(int startX, int startY, int hue) {
    // this is the constructor routine, called when we make a new object of our particle class defined in the header
    position = ofVec2f(startX, startY);
    force = ofVec2f(0, 0.02);
    direction = ofVec2f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)).normalize();
    
    size = 5;
    moveSpeed = 0.0f;
    color.setHsb(hue, 255, 255, 255);

}

particle::~particle() {
    // destructor
}

void particle::update() {
    position += direction * moveSpeed;
    //direction += force;   
}

void particle::draw() {
    ofSetColor(color);
    ofDrawCircle(position, size);
}

void particle::drawTail() {
    radialGradientCircle();
}

bool particle::IsToBeDeleted() {
    if (isOutOfBounds(position.x, position.y)) return true;
    return false;
   
}

void particle::changeBrightness(float amount) {
    float brightness = color.getBrightness();
    float myHue = color.getHue();

    if (brightness > 20) {
         color.setBrightness( brightness += amount );
        color.setHue(myHue -= 0.5);
    }
}

void particle::changeSize(float amount) {
    if (size > 1) {
        size += amount;
    }
}

void particle::radialGradientCircle()
{
    ofSetColor(150, 150 , 0);
    ofDrawCircle(position, size*3);
}

void particle::radialGradientCircle2()
{
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    ofVec3f vert;

    ofColor innerColor(0, 255, 0, 50);
    ofColor outerColor(0, 255, 0, 0);
    int slices = 25;
    float radius = 50;
    float incr = (float)(2 * PI / slices);

    vert = ofVec3f(position.x, position.y, 0);
    mesh.addVertex(vert);
    mesh.addColor(innerColor);

    //mesh.addColor(outerColor);

    for (int i = 0; i < slices; i++)
    {
        float angle = incr * i;

        float x = (float)cos(angle) * radius;
        float y = (float)sin(angle) * radius;


        vert = ofVec3f(position.x + x, position.y + y, 0);
        mesh.addVertex(vert);
        mesh.addColor(outerColor);

        /*std::cout << "angle: " << angle << " x: " << x << " y: " << y << std::endl;*/
    }

    vert = ofVec3f(position.x + radius, position.y, 0);
    mesh.addVertex(vert);
    mesh.addColor(outerColor);

    
    mesh.draw();

   /* std::cout << "inc: " << incr << std::endl;*/
}