#pragma once

#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/3d/plane.hpp"
 
struct ModelFactory {
	// Creates a cube model
	// The return is a pointer, remember to add to a Scene OR manually delete it
	static Cube* create_cube(const Model::Config& conf) noexcept;
	// Creates a Plane model
	// The return is a pointer, remember to add to a Scene OR manually delete it
	static Plane* create_plane(const Model::Config& conf) noexcept;
};
