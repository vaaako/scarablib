#include "scarablib/types/raycast.hpp"


Raycast::Rayhit Raycast::cast(const Ray& ray, const std::function<bool(const vec3<int>& position, const Rayhit& hit_result)> hit_condition, float max_distance, const float step) {
	Rayhit hit_info;
	vec3<float> current_point = ray.origin;

	for(float distance = 0.0f; distance < max_distance; distance += step) {
		current_point = ray.origin + ray.direction * distance;

		if(hit_condition(current_point, hit_info)) {
			hit_info.hit = true;
			hit_info.distance = distance;
			hit_info.point = current_point;
			// Normal not set here
			return hit_info;
		}
	}

	// No hit
	return hit_info;
}

// Raycast::Rayhit Raycast::cast(const Ray& ray, const std::function<bool(const vec3<int>& position)> hit_condition, float max_distance) {
// 	Rayhit hit_info;
//
// 	// DDA traversal algorithm
// 	vec3<float> ray_position = ray.origin;
// 	vec3<float> current_pos = {
// 		ray.origin.x,
// 		ray.origin.y,
// 		ray.origin.z
// 	};
// 	const vec3<float> ray_direction = glm::normalize(ray.direction);
//
// 	const vec3<int> step = {
// 		(ray_direction.x >= 0) ? 1 : -1,
// 		(ray_direction.y >= 0) ? 1 : -1,
// 		(ray_direction.z >= 0) ? 1 : -1
// 	};
//
// 	const vec3<float> next_boundary = {
// 		(step.x > 0)  ? (ray.origin.x + 1.0f) : ray.origin.x,
// 		(step.y > 0)  ? (ray.origin.y + 1.0f) : ray.origin.y,
// 		(step.z > 0)  ? (ray.origin.z + 1.0f) : ray.origin.z
// 	};
//
// 	vec3<float> t_max = {
// 		(next_boundary.x - ray.origin.x) / ray_direction.x,
// 		(next_boundary.y - ray.origin.y) / ray_direction.y,
// 		(next_boundary.z - ray.origin.z) / ray_direction.z
// 	};
// 	const vec3<float> t_delta = {
// 		static_cast<float>(step.x) / ray_direction.x,
// 		static_cast<float>(step.y) / ray_direction.y,
// 		static_cast<float>(step.z) / ray_direction.z
// 	};
//
// 	float distance_traveled = 0.0f;
// 	while(distance_traveled < max_distance) {
// 		if(hit_condition(current_pos)) {
// 			return Rayhit {
// 				.hit = true,
// 				.distance = distance_traveled,
// 				.point = ray_position,
// 				.normal = ray_direction
// 				// .position = current_pos
// 			};
// 		}
//
// 		// Step to the next grid broundary
// 		if(t_max.x < t_max.y) {
// 			if(t_max.x < t_max.z) {
// 				distance_traveled = t_max.x;
// 				t_max.x += t_delta.x;
// 				current_pos.x += step.x;
// 				hit_info.normal = { -step.x, 0.0f, 0.0f };
// 			} else {
// 				distance_traveled = t_max.z;
// 				t_max.z += t_delta.z;
// 				current_pos.z += step.z;
// 				hit_info.normal = { 0.0f, 0.0f, -step.z };
// 			}
// 		} else {
// 			if(t_max.y < t_max.z) {
// 				distance_traveled = t_max.y;
// 				t_max.y += t_delta.y;
// 				current_pos.y += step.y;
// 				hit_info.normal = { 0.0f, -step.y, 0.0f };
// 			} else {
// 				distance_traveled = t_max.z;
// 				t_max.z += t_delta.z;
// 				current_pos.z += step.z;
// 				hit_info.normal = { 0.0f, 0.0f, -step.z };
// 			}
// 		}
// 		ray_position = ray.origin + ray_direction * distance_traveled;
// 	}
// 	return hit_info;
// }

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
