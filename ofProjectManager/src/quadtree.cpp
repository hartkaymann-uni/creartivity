#include "quadtree.h"


namespace quadtree {
	node::node() : node( -1, glm::vec2( 0.f ), glm::vec2( 0.f ), nullptr ) {	}

	node::node( const int level, glm::vec2 position, glm::vec2 dimensions, std::vector<Particle>* objs )
		: level( level ),
		position( position ),
		dimensions( dimensions ),
		children{ nullptr, nullptr, nullptr, nullptr },
		objects( objs )
	{
		indices.reserve( NO_CHILDREN );
	}


	void node::update()
	{
		indices.clear();
		indices.shrink_to_fit();

		children[0] = nullptr;
		children[1] = nullptr;
		children[2] = nullptr;
		children[3] = nullptr;

		for (size_t i = 0; i < objects->size(); i++) {
			insert( i );
		}
	}

	void node::draw() const
	{
		ofSetColor( ofColor::white );
		ofNoFill();
		ofDrawRectangle( position, dimensions.x - 5, dimensions.y - 5 );
		ofDrawRectangle( 10 * level, 10 * level, 50, 50 );
		ofFill();

		if (children[0] != nullptr) {
			for (const auto& child : children) {
				child->draw();
			}
		}
	}

	void node::split()
	{
		glm::vec2 d = dimensions / 2.0;

		glm::vec2 NW = position;
		glm::vec2 NE = position + d.x;
		glm::vec2 SW = position + d.y;
		glm::vec2 SE = position + d;

		children[0] = std::make_unique<node>( level + 1, NW, d, objects );
		children[0] = std::make_unique<node>( level + 1, NE, d, objects );
		children[0] = std::make_unique<node>( level + 1, SW, d, objects );
		children[0] = std::make_unique<node>( level + 1, SE, d, objects );

	}

	void node::insert( const int id )
	{
		// If node has split
		if (children[0] != nullptr)
		{
			if (children[0]->contains( id )) children[0]->insert( id );
			else if (children[1]->contains( id )) children[1]->insert( id );
			else if (children[2]->contains( id )) children[2]->insert( id );
			else if (children[3]->contains( id )) children[3]->insert( id );

			return;
		}

		// Insert item here
		indices.push_back( id );

		// Split if required
		if (indices.size() > NODE_CAPACTIY && level < NODE_MAX_DEPTH) {
			split();

			for (const auto& index : indices) {
				for (const auto& child : children) {
					if (child->contains( index )) {
						child->insert( index );
					}
				}
			}

			// Remove indices from this node
			indices.clear();
			indices.shrink_to_fit();
		}
	}

	bool node::contains( const int id ) const
	{
		glm::vec2 pos = objects->at( id ).pos;

		// Basic square collision check
		if (pos.x > position.x && pos.x < position.x + dimensions.x && pos.y > position.y
			&& pos.y < position.y + dimensions.y)
		{
			return true;
		}

		return false;
	}

}