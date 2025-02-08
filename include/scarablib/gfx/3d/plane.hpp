#pragma once

#include "scarablib/gfx/model.hpp"
#include <vector>

// This is a class to make a plane model.
// WARNING: Do not use this class directly, use ModelFactory::create_cube(const ModelConf& conf)
struct Plane : public Model {
	// WARNING: Do not use this class directly, use ModelFactory::create_cube(const ModelConf& conf)
	Plane(const Model::Config& conf, const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) noexcept;
};
