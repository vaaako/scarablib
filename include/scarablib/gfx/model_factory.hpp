#pragma once

#include "scarablib/gfx/3d/billboard.hpp"
#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/3d/plane.hpp"
 
struct ModelFactory {
	// Creates a cube model.
	// The return is a pointer, remember to add to a Scene OR manually delete it
	static Cube* create_cube(const Model::Config& conf) noexcept;
	// Creates a Plane model.
	// The return is a pointer, remember to add to a Scene OR manually delete it
	static Plane* create_plane(const Model::Config& conf) noexcept;

	// Creates a Plane model.
	// The return is a pointer, remember to add to a Scene OR manually delete it,
	// The difference between a Billboard and a Plane is that a Billboard will always face the camera.
	// Also the billboard can have directional textures
	static Billboard* create_billboard(const Model::Config& conf) noexcept;
};
