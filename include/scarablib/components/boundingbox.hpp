#pragma once

#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"

// This class is a AABB of a 3D model.
// It can be used for hitbox and collision detection
struct BoundingBox {
	// Minimum corner of the bounding box in local space
	vec3<float> local_min = vec3<float>(FLT_MAX);
	// Minimum corner of the bounding box in local space
	vec3<float> local_max = vec3<float>(FLT_MAX);
	// Maximum corner of the bounding box in world space
	vec3<float> min = vec3<float>(FLT_MAX);
	// Size of the bounding box (max - min)
	vec3<float> max = vec3<float>(-FLT_MAX);
	// Minimum corner of the bounding box in world space
	vec3<float> size = vec3<float>(0.0f);

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

	// Draws the bounding box of the model in local space.
	// This is only intented to be used as a debug tool
	void draw_local_bounds(const Camera& camera, const Color& color = Colors::RED, const bool stripped = false) noexcept;
	// Draws the bounding box of the model in world space.
	// This is only intented to be used as a debug tool
	void draw_world_bounds(const Camera& camera, const Color& color = Colors::RED, const bool stripped = false) noexcept;

	// Returns the 8 corners of the bounding box in local space.
	// These corners are used for rendering, collision detection, and transformations
	inline std::vector<vec3<float>> get_local_corners() const noexcept {
		return {
			{ this->local_min.x, this->local_min.y, this->local_min.z },
			{ this->local_max.x, this->local_min.y, this->local_min.z },
			{ this->local_max.x, this->local_min.y, this->local_max.z },
			{ this->local_min.x, this->local_min.y, this->local_max.z },
			{ this->local_min.x, this->local_max.y, this->local_min.z },
			{ this->local_max.x, this->local_max.y, this->local_min.z },
			{ this->local_max.x, this->local_max.y, this->local_max.z },
			{ this->local_min.x, this->local_max.y, this->local_max.z }
		};
	}

	private:
		void draw(const bool world, const Camera& camera, const Color& color, const bool stripped) noexcept;
};

