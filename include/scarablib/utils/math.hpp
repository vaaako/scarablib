#pragma once

#include "scarablib/proper/vector/vec3.hpp"
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

// Helper namespace with methods related to string manipulation
namespace MathHelper {
	// Returns true if `a` can be considered equal to `b` with optional tolerance
	constexpr inline bool compare_floats(const float a, const float b, const float tolerance = 0.00001f) {
		return std::abs(a - b) < tolerance;
	}

	// returns true if `v1` can be considered equal to `v2`
	constexpr inline bool is_near(const float v1, const float v2) {
		return std::fabs(v1 - v2) < 0.01f;
	}

	// Calculate horizontal orbit (around z-axis) around a center position
	inline vec3<float> orbitate_z(const vec3<float> center_position, const float velocity, const float radius = 10.0f) {
		float angle = glm::radians(velocity);
		return center_position + vec3<float>(radius * std::cos(angle), 0.0f, radius * std::sin(angle));
	}

	// Calculate vertical orbit (around x-axis) around a center position
	inline vec3<float> orbitate_x(const vec3<float> center_position, const float velocity, const float radius = 10.0f) {
		float angle = glm::radians(velocity);
		return center_position + vec3<float>(0.0f, radius * std::cos(angle), radius * std::sin(angle));
	}

	// Calculate orbit around the y-axis (xz-plane)
	inline vec3<float> orbitate_y(const vec3<float> center_position, const float velocity, const float radius = 10.0f) {
		float angle = glm::radians(velocity);
		return center_position + vec3<float>(radius * std::cos(angle), radius * std::sin(angle), radius * std::sin(angle));
	}

	// Return the distance between two positions
	inline float distance_between(const vec3<float>& pos1, const vec3<float> pos2) {
		return glm::distance(pos1.to_glm(), pos2.to_glm());
	}
}
