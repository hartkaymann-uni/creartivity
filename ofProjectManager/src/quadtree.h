#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "ofMain.h"

#include "particle.h"


#define NO_CHILDREN 4
#define NODE_CAPACTIY 5
#define NODE_MAX_DEPTH 5

namespace quadtree {
	class node {
	public:

		node();
		node(const int level, glm::vec2 position, glm::vec2 dimensions, std::vector<Particle>* objs) ;
		
		void update();
		void draw() const;

	private:
		int level;
		
		glm::vec2 position;
		glm::vec2 dimensions;

		std::unique_ptr<node> children[NO_CHILDREN];
		std::vector<int> indices;

		std::vector<Particle>* objects;

		void split();
		void insert( const int id );
		bool contains( const int id ) const;

	};
}
