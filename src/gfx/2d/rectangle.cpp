#include "scarablib/gfx/2d/rectangle.hpp"
#include "scarablib/geometry/geometry_factory.hpp"

Rectangle::Rectangle() noexcept
	: Sprite(GeometryFactory::make_rectangle_vertices()) {}

