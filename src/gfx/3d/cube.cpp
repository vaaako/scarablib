#include "scarablib/gfx/3d/cube.hpp"
#include "scarablib/gfx/geometry_factory.hpp"

Cube::Cube() noexcept
	: Model(GeometryFactory::make_cube_vertices(), std::vector<uint8> {
			// Front face
			0, 1, 2,
			0, 2, 3,

			// Back face
			4, 5, 6,
			4, 6, 7,

			// Top face
			8, 9, 10,
			8, 10, 11,

			// Bottom face
			12, 13, 14,
			12, 14, 15,

			// Left face
			16, 17, 18,
			16, 18, 19,

			// Right face
			20, 21, 22,
			20, 22, 23
		}
	) {}
