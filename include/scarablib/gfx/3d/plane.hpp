#pragma once

#include "scarablib/geometry/model.hpp"

// Plane model object
struct Plane : public Model {
	enum Type : uint8 {
		// Only one plane
		SINGLE_PLANE,
		// Two crossed planes in X shape
		CROSSED_PLANE,
		// Four crossed planes in X shape.
		// Where two planes are opposite to each other
		FOUR_CROSSED_PLANE
	};

	// Type of plane to build
	Plane(const Plane::Type type) noexcept;
};
