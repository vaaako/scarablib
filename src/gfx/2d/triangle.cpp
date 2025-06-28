#include "scarablib/gfx/2d/triangle.hpp"
#include "scarablib/geometry/geometry_factory.hpp"

Triangle::Triangle() noexcept
	: Sprite(GeometryFactory::make_triangle_vertices()) {}

