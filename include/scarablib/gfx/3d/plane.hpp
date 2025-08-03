#pragma once

#include "scarablib/geometry/model.hpp"

// Plane model object
struct Plane : public Model {
	// The type of the plane
	// - SINGLE_PLANE: Only one plane
	// - CROSSED_PLANE: Two crossed planes in X shape
	// - FOUR_CROSSED_PLANE: Four crossed planes in X shape. Where two planes are opposite to each other
	//
	// FOUR_CROSSED_PLANE has a better effect but only works with CULLING_FACE enabled
	enum Type : uint8 {
		SINGLE_PLANE,
		CROSSED_PLANE,
		FOUR_CROSSED_PLANE
	};

	// Type of plane to build
	Plane(const Plane::Type type) noexcept;
};
