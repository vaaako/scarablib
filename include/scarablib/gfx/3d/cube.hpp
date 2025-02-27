#pragma once

#include "scarablib/gfx/3d/model.hpp"

// Class for cube model
struct Cube : public Model {
	// WARNING: Do not use this constructor, use ModelFactory
	Cube(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
};
