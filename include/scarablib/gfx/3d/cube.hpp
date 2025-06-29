#pragma once

#include "scarablib/gfx/3d/model.hpp"

// Cube model object
struct Cube : public Model {

	// Enum used to define wich faces render
	enum Face : uint8 {
		FRONT  = 1 << 0, // -Z
		BACK   = 1 << 1, // +Z
		TOP    = 1 << 2, // +Y
		BOTTOM = 1 << 3, // -Y
		LEFT   = 1 << 4, // +X,
		RIGHT  = 1 << 5  // -X 
	};

	Cube(const uint8 face_mask = 0b111111) noexcept; // Render all faces by default
};
