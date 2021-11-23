#include "octree.h"


Octree::node::node() {

}

Octree::node::node( std::vector < Particle > obj) {

	objects.insert( obj.end(), obj.begin(), obj.end() );
}

void Octree::node::build()
{
	// <= 1 objects
	if (objects.size() <= 1) {
		return;
	}
	
	// Too small
	glm::vec3 dimensions;
	for (int i = 0; i < 3; i++) {
		if (dimensions[i] < MIN_BOUNDS) {
			return;
		}
	}

	// Create Regions
	for (int i = 0; i < NO_CHILDREN; i++) {
		children[i] = new node();
		children[i]->parent = this;
	}
}