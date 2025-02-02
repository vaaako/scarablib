#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/model.hpp"

Cube::Cube(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(conf, vertices, indices) {}
