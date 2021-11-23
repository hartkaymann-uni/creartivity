#pragma once

#define NO_CHILDREN 8
#define MIN_BOUNDS .5

#include <vector>
#include <queue>
#include <stack>
#include <glm/glm.hpp>

#include "ofMain.h"
#include "list.hpp"



struct Particle {
	glm::vec4 pos;
	glm::vec4 vel;
	ofFloatColor color;
};

namespace Octree {
	enum class Octant : unsigned char {
		O1 = 0x01, // = 0b00000001
		O2 = 0x02, // = 0b00000010
		O3 = 0x04, // = 0b00000100
		O4 = 0x08, // = 0b00001000
		O5 = 0x10, // = 0b00010000
		O6 = 0x20, // = 0b00100000
		O7 = 0x40, // = 0b01000000
		O8 = 0x80, // = 0b10000000
	};

	class node {
	public:
		node* parent;

		node* children[NO_CHILDREN];

		unsigned char activeOctants;

		bool hasChildren = false;

		std::vector< Particle > objects;

		node();
		node( std::vector < Particle > );
		void build();
	};
}