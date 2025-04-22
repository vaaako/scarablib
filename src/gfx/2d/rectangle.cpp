#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/gfx/geometry_factory.hpp"
#include <cstdio>

Rectangle::Rectangle() noexcept
	: Sprite(GeometryFactory::make_rectangle_vertices()) {}

