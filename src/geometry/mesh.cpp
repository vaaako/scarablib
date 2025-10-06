#include "scarablib/geometry/mesh.hpp"

// Indices attributes are not needed to set here
// I will not make Bounding Box here because this is probably a 2D model
Mesh::Mesh(const std::vector<Vertex>& vertices) noexcept {
	this->vertexarray = ResourcesManager::get_instance()
		.acquire_vertexarray(vertices, std::vector<uint8>{});
	this->vertexarray->add_attribute<float>(2, true);
}

Mesh::~Mesh() noexcept {
	if(this->physics) {
		delete this->physics;
	}
	if(this->bbox) {
		delete this->bbox;
	}
}

