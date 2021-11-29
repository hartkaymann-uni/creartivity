#pragma once

#include <glm/glm.hpp>
#include "ofMain.h"

struct Particle {
	glm::vec4 pos;
	glm::vec4 vel;
	ofFloatColor color;
};