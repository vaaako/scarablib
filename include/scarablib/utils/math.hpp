#pragma once

#include "scarablib/typedef.hpp"
#include <cmath>

// Helper namespace with methods related to string manipulation
namespace ScarabMath {
	// Convert degrees to radians
	[[nodiscard]] inline constexpr float radians(const float degrees) noexcept {
		return degrees * static_cast<float>(0.01745329251994329576923690768489);
	}

	// Returns true if `a` can be considered equal to `b` with optional tolerance
	[[nodiscard]] inline constexpr bool compare_floats(const float a, const float b, const float tolerance = std::numeric_limits<float>::epsilon()) {
		return std::fabs(a - b) < tolerance;
	}

	// Safe comparison for near-zero floating-point values
	template <typename T>
	[[nodiscard]] inline constexpr bool is_near_zero(const T value) noexcept {
		return std::fabs(value) < 1e-6f; // Small tolerance value for floating-point comparison
 
	}


	// VECTORS //

	// Calculate horizontal orbit (around z-axis) around a center position
	[[nodiscard]] inline constexpr vec3<float> orbitate_z(const vec3<float>& center_position, const float velocity, const float radius = 5.0f) {
		const float angle = ScarabMath::radians(velocity);
		return center_position + vec3<float>(radius * std::cos(angle), 0.0f, radius * std::sin(angle));
	}

	// Calculate vertical orbit (around x-axis) around a center position
	[[nodiscard]] inline constexpr vec3<float> orbitate_x(const vec3<float>& center_position, const float velocity, const float radius = 5.0f) {
		const float angle = ScarabMath::radians(velocity);
		return center_position + vec3<float>(0.0f, radius * std::cos(angle), radius * std::sin(angle));
	}

	// Calculate orbit around the y-axis (xz-plane)
	[[nodiscard]] inline constexpr vec3<float> orbitate_y(const vec3<float>& center_position, const float velocity, const float radius = 5.0f) {
		const float angle = ScarabMath::radians(velocity);
		const float sin_angle = std::sin(angle);
		return center_position + vec3<float>(radius * std::cos(angle), radius * sin_angle, radius * sin_angle);
	}

	// Return the distance between two positions
	[[nodiscard]] inline constexpr float distance_between(const vec3<float>& pos1, const vec3<float>& pos2) {
		return glm::length(pos1) - glm::length(pos2);
	}
}
