#pragma once

// WARNING: Not a working raycast yet

#include "scarablib/geometry/triangle.hpp"
#include <limits>

class Raycast {
	public:
		struct Ray {
			vec3<float> origin;
			vec3<float> direction;
		};

		struct Rayhit {
			bool hit = false;
			float distance = std::numeric_limits<float>::max();
			vec3<float> point;
			vec3<float> normal;
		};

		// Fast Ray-Triangle intersection using the Moller-Trumbore algorithm.
		// Returns true if the ray intersects the triangle, false otherwise.
		// - `ray`: The ray to test.
		// - `triangle`: The triangle to test against.
		// - `out_distance`: The distance from the ray origin to the intersection point.
		static bool intersects_triangle(const Ray& ray, const MeshTriangle& triangle, float& out_distance);
	private:
};
