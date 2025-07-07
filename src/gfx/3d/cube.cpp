#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/geometry/geometry_factory.hpp"

Cube::Cube(const uint8 face_mask) noexcept
	: Model() {

	auto geometry = GeometryFactory::make_cube_faces(face_mask);
	this->set_geometry(geometry.first, geometry.second);
}
