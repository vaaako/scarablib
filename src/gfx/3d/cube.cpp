#include "scarablib/gfx/3d/cube.hpp"

Cube::Cube(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(vertices, indices) {}
