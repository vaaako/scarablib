#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/gfx/geometry_factory.hpp"
#include "scarablib/gfx/3d/model.hpp"

Plane::Plane() noexcept
	: Model(GeometryFactory::make_plane_vertices(), std::vector<uint8> { 0, 1, 2, 0, 2, 3 }) {}

