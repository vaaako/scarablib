#pragma once

#include "scarablib/gfx/model.hpp"

// This is a class to make a cube model
// WARNING: Do not use this class directly, use ModelFactory::create_cube(const Model::Config& conf)
struct Cube : public Model {
	// WARNING: Do not use this class directly, use ModelFactory::create_cube(const Model::Config& conf)
	Cube(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
};
