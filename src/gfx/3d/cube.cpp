#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/geometry_factory.hpp"

Cube::Cube(const uint8 face_mask) noexcept
	: Model() {

	const std::vector<Vertex> vertices = GeometryFactory::make_cube_vertices();
	std::vector<uint8> indices;

	constexpr uint8 indices_index[6] = { 0, 1, 2, 0, 2, 3 };

	// Offset of one mesh to another
	uint32 vertex_offset = 0;

	// Each face adds 6 vertices, 6 indices
	for(int v = 0; v < 6; v++) {
		// Hide face if included
		if((face_mask & (1 << v)) == 0) {
			continue;
		}

		for(int i = 0; i < 6; i++) {
			indices.push_back(static_cast<uint8>(indices_index[i] + vertex_offset));
		}
		// Amount of vertices to skip (next face)
		vertex_offset += 4;
	}

	this->set_geometry(vertices, indices);
}
