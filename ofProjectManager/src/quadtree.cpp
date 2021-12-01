#include "quadtree.h"


namespace quadtree {
	node::node() : node( -1, 0.f, 0.f, 0.f, 0.f, nullptr ) {	}

	node::node( const int level, const int x, const int y, const int width, const int height, std::vector<Particle>* objs )
		: level( level ),
		position( glm::vec2( x, y ) ),
		dimensions( glm::vec2( width, height ) ),
		children{ nullptr, nullptr, nullptr, nullptr },
		objects( objs )
	{
		indices.reserve( NO_CHILDREN );
	}

	void node::build()
	{
		
	}

	void node::update()
	{

		/*if (children[0] != nullptr) {
			for (auto& child : children) {
				child->update();
			}
			return;
		}*/

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
		ofSetColor( 255 );
		ofNoFill();
		ofDrawRectangle( position.x, position.y, dimensions.x, dimensions.y );
		ofFill();

		if (children[0] != nullptr) {
			for (const auto& child : children) {
				child->draw();
			}
		}
	}

	void node::split()
	{
		glm::vec2 d = dimensions * 0.5f;

		glm::vec2 NW = position;
		glm::vec2 NE = position + glm::vec2( d.x, 0.f );
		glm::vec2 SW = position + glm::vec2( 0.f, d.y );
		glm::vec2 SE = position + d;

		children[0] = std::make_unique<node>( level + 1, NW.x, NW.y, d.x, d.y, objects );
		children[1] = std::make_unique<node>( level + 1, NE.x, NE.y, d.x, d.y, objects );
		children[2] = std::make_unique<node>( level + 1, SW.x, SW.y, d.x, d.y, objects );
		children[3] = std::make_unique<node>( level + 1, SE.x, SE.y, d.x, d.y, objects );
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

	void node::reset()
	{
		position = glm::vec2( 0.f );
		dimensions = glm::vec2( 0.f );
		indices.clear();
		indices.shrink_to_fit();

		children[0] = nullptr;
		children[1] = nullptr;
		children[2] = nullptr;
		children[3] = nullptr;
	}

}