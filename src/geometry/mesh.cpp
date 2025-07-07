#include "scarablib/geometry/mesh.hpp"

// Indices attributes are not needed to set here
// I will not make Bounding Box here because this is probably a 2D model
Mesh::Mesh(const std::vector<Vertex>& vertices) noexcept {
	this->bundle.make_vao(vertices, std::vector<uint8> {});
}

Mesh::~Mesh() noexcept {
	delete this->physics;
	delete this->bbox;
}

