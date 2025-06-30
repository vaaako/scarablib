#include "scarablib/types/raycast.hpp"

bool Raycast::intersects_triangle(const Ray& ray, const MeshTriangle& triangle, float& out_distance) {
	constexpr float EPSILON = 0.0000001f;

	const vec3<float> edge1 = triangle.v1 - triangle.v0;
	const vec3<float> edge2 = triangle.v2 - triangle.v0;
	const vec3<float> h = glm::cross(ray.direction, edge2);

	const float a = glm::dot(edge1, h);

	if(a > -EPSILON && a < EPSILON) {
		// Ray is parallel to the triangle
		return false;
	}

	const float f = 1.0f / a;
	const vec3<float> s = ray.origin - triangle.v0;
	const float u = f * glm::dot(s, h);

	if(u < 0.0f || u > 1.0f) {
		return false;
	}

	const vec3<float> q = glm::cross(s, edge1);
	const float v = f * glm::dot(ray.direction, q);

	if (v < 0.0f || u + v > 1.0f) {
		return false;
	}

	// At this stage we can compute t to find out where the intersection point is on the line
	const float t = f * glm::dot(edge2, q);
	// Ray intersection
	if(t > EPSILON) {
		out_distance = t;
		return true;
	}

	// Line intersection but not a ray intersection.
	return false;
}
