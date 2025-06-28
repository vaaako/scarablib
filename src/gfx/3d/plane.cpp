#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/geometry/geometry_factory.hpp"

Plane::Plane() noexcept
	: Model(GeometryFactory::make_plane_vertices(), std::vector<uint8> { 0, 1, 2, 0, 2, 3 }) {}

