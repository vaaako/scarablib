#pragma once

// WARNING: Not a working raycast yet

#include "scarablib/geometry/triangle.hpp"
#include <functional>
#include <limits>

class Raycast {
	public:
		struct Ray {
			// Where the ray starts
			vec3<float> origin;
			// Forward
			vec3<float> direction;
		};

		struct Rayhit {
			// Has the ray hit something
			bool hit = false;
			// Distance
			float distance = std::numeric_limits<float>::max();
			// Where the ray hits
			vec3<float> point;
			// Unit vector that is perpendicular to the surface at the point of intersection
			vec3<float> normal;
		};

		// A general purpose raycast.
		// Casts a ray by stepping along it and using a callback.
		// Normal is not set here
		static Rayhit cast(const Ray& ray, const std::function<bool(const vec3<int>& position, const Rayhit& hit_result)> hit_condition, float max_distance = 10.0f, const float step = 0.1f);

		// Fast Ray-Triangle intersection using the Moller-Trumbore algorithm.
		// Returns true if the ray intersects the triangle, false otherwise.
		// - `ray`: The ray to test.
		// - `triangle`: The triangle to test against.
		// - `out_distance`: The distance from the ray origin to the intersection point.
		static bool intersects_triangle(const Ray& ray, const MeshTriangle& triangle, float& out_distance);
	private:
};
