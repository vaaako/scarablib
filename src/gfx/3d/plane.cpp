#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/gfx/model.hpp"

Plane::Plane(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept
	: Model(vertices, indices) {}

