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
		// Where two planes are opposite to each other.
		// In order to work, Cullface is enabled when redering this Mesh
		FOUR_CROSSED_PLANE
	};

	// Plane type
	const Plane::Type type = Plane::Type::SINGLE_PLANE;

	// Type of plane to build
	Plane(const Plane::Type type) noexcept;

	// Overrides to disable Cullface when drawing
	void draw_logic(const Camera &camera) noexcept override;
};
