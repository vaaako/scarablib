#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"

// This class is a AABB of a 3D model.
// It can be used for hitbox and collision detection
struct BoundingBox {
	// Minimum corner of the bounding box
	vec3<float> size = vec3<float>(0.0f);
	// Maximum corner of the bounding box
	vec3<float> min = vec3<float>(FLT_MAX);
	// Size of the bounding box (max - min)
	vec3<float> max = vec3<float>(-FLT_MAX);

	// If using this constructor, please initialize the bounding box with `calculate_local_bounds`
	BoundingBox() noexcept = default;
	// Computes the bounding box size using the model's vertices.
	// This initializes the bounding box in local space
	BoundingBox(const std::vector<Vertex>& vertices) noexcept;
	~BoundingBox() noexcept = default;

	// Get the center position of the bounding box
	inline vec3<float> get_center_position() const noexcept {
		return this->min + (this->size * 0.5f);
	}

	// Computes the center of bounding box in local space using the model's vertices.
	// Generally used when creating a new bounding box
	void calculate_local_bounds(const std::vector<Vertex>& vertices) noexcept;

	// Updates the center of bounding box in world space using the model's transformation matrix.
	// This ensures the bounding box aligns with the model's position, scale, and rotation
	void update_world_bounds(const glm::mat4& model_matrix) noexcept;


	// Check collision with another AABB
	inline bool collides_with(const BoundingBox& other) const noexcept {
		return (
			this->min.x <= other.max.x && this->max.x >= other.min.x &&
			this->min.y <= other.max.y && this->max.y >= other.min.y &&
			this->min.z <= other.max.z && this->max.z >= other.min.z
		);
	}

	// Check collision (AABB) with a point (e.g., camera position)
	inline bool collides_with_point(const vec3<float>& point) const noexcept {
		return (
			point.x >= this->min.x && point.x <= this->max.x &&
			point.y >= this->min.y && point.y <= this->max.y &&
			point.z >= this->min.z && point.z <= this->max.z
		);
	}

	// Check collision (AABB) with an sphere
	inline bool collides_with_sphere(const vec3<float>& center, const float radius) const noexcept {
		// Find closest point on AABB to sphere center
		const vec3<float> closest_point = glm::clamp(center, this->min, this->max);
		const float distance = glm::length(center - closest_point);
		return distance <= radius;
	}


	// Returns the 8 corners of the bounding box in world space.
	// These corners are used for rendering, collision detection, and transformations
	inline std::vector<vec3<float>> get_bounding_corners() const noexcept {
		return {
			{ this->min.x, this->min.y, this->min.z },
			{ this->max.x, this->min.y, this->min.z },
			{ this->max.x, this->min.y, this->max.z },
			{ this->min.x, this->min.y, this->max.z },
			{ this->min.x, this->max.y, this->min.z },
			{ this->max.x, this->max.y, this->min.z },
			{ this->max.x, this->max.y, this->max.z },
			{ this->min.x, this->max.y, this->max.z }
		};
	}
};

