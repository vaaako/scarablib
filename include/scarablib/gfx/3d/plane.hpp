#pragma once

#include "scarablib/gfx/3d/model.hpp"

// Class for plane model
struct Plane : public Model {
	// WARNING: Do not use this constructor, use ModelFactory
	Plane(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
};
