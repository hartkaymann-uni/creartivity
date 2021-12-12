#pragma once

#include "ofMain.h"
#include "../helper.h"

class particle
{
public:
    ofColor color;
    float size;
    float moveSpeed;
    glm::vec2 force, position, direction;

    void update();
    void draw();
    void drawTail();
    bool IsToBeDeleted();

    particle(int x, int y, int hue);
    ~particle();

private:
    void changeSize(float amount);
    void changeBrightness(float amount);
    void radialGradientCircle();
    void radialGradientCircle2();
};

