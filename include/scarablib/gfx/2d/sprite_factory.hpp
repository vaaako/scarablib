#pragma once
 
#include "scarablib/gfx/2d/circle.hpp"
#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/gfx/2d/triangle.hpp"

struct SpriteFactory {
	// Creates a Rectangle shape.
	// The return is a pointer, remember to add to a Scene OR manually delete it
	static Rectangle* create_rectangle() noexcept;
	// Creates a Triangle shape.
	// The return is a pointer, remember to add to a Scene OR manually delete it
	static Triangle* create_triangle() noexcept;
	// Creates a Circle shape.
	// The return is a pointer, remember to add to a Scene OR manually delete it
	static Circle* create_circle() noexcept;
};
