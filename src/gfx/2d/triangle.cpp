#include "scarablib/gfx/2d/triangle.hpp"
#include "scarablib/gfx/geometry_factory.hpp"

Triangle::Triangle() noexcept
	: Sprite(GeometryFactory::make_triangle_vertices()) {}

