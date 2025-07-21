#pragma once

#include "scarablib/geometry/model.hpp"

// Cube model object
struct Cube : public Model {

	// Enum used to define wich faces render
	enum Face : uint8 {
		FRONT  = 1 << 0, // -Z
		BACK   = 1 << 1, // +Z
		TOP    = 1 << 2, // +Y
		BOTTOM = 1 << 3, // -Y
		RIGHT  = 1 << 4, // +X
		LEFT   = 1 << 5, // -X
	};

	Cube(const uint8 face_mask = 0b111111) noexcept; // Render all faces by default
};
