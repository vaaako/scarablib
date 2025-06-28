#include "scarablib/geometry/mesh.hpp"

// Indices attributes are not needed to set here
// I will not make Bounding Box here because this is probably a 2D model
Mesh::Mesh(const std::vector<Vertex>& vertices) noexcept {
	this->bundle.make_vao(vertices, std::vector<uint8> {});

	// Is not indices, but will have a similar use
	this->indices_length = static_cast<GLsizei>(vertices.size());
}

Mesh::~Mesh() noexcept {
	delete this->physics;
	delete this->bbox;
}

