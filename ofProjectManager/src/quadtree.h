#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "ofMain.h"

#define NO_CHILDREN 4
#define NODE_CAPACTIY 10
#define NODE_MAX_DEPTH 6

namespace quadtree {

	struct Particle {
		glm::vec2 pos;
		glm::vec2 vel;
		ofFloatColor color;
	};

	class node {
	public:

		node();
		node( const int level, const int x, const int y, const int width, const int height, std::vector<Particle>* objs );

		void build();
		void update();
		void draw() const;

		std::vector<size_t> indices;

	private:
		int level;

		glm::vec2 position;
		glm::vec2 dimensions;

		std::unique_ptr<node> children[NO_CHILDREN];

		std::vector<Particle>* objects;

		void split();
		void insert( const int id );
		bool contains( const int id ) const;
		void reset();

	};
}
