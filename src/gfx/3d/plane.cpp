#include "scarablib/gfx/3d/plane.hpp"
#include "scarablib/geometry/geometry_factory.hpp"

Plane::Plane(const Plane::Type type) noexcept : Model() {
	switch (type) {
		case Plane::Type::SINGLE_PLANE:
			this->set_geometry(
				GeometryFactory::make_plane_vertices(),
				std::vector<uint8> {
					0, 1, 2, 0, 2, 3
				}
			);
			break;
		case Plane::Type::CROSSED_PLANE:
			this->set_geometry(
				GeometryFactory::make_crossedplane_vertices(),
				std::vector<uint8> {
					0, 1, 2, 2, 3, 0, // First quad
					4, 5, 6, 6, 7, 4  // Second quad
				}
			);
			break;
		case Plane::Type::FOUR_CROSSED_PLANE:
			this->set_geometry(
				GeometryFactory::make_fourcrossedplane_vertices(),
				std::vector<uint8> {
					 0,  1,  2,  0,  2,  3, // Quad 1
					 4,  5,  6,  4,  6,  7, // Quad 2
					 8,  9, 10,  8, 10, 11, // Quad 3
					12, 13, 14, 12, 14, 15  // Quad 4
				}
			);
			break;

		default:
			break;
	}
}

