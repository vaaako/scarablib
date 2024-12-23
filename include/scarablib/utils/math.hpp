#pragma once

#include "scarablib/typedef.hpp"
#include <cmath>

// Helper namespace with methods related to string manipulation
namespace ScarabMath {
	static float angles[360];
	static float sin_angles[360];
	static float cos_angles[360];

	constexpr float EPSILON = 1e-6f; // Small tolerance value for floating-point comparison

	// Initialize arrays
	inline void initialize() noexcept {
		for(uint32 i = 0; i < 360; i++) {
			angles[i]     = i * static_cast<float>(0.01745329251994329576923690768489);
			sin_angles[i] = std::sin(angles[i]);
			cos_angles[i] = std::cos(angles[i]);
		}
	}

	// Convert degrees to radians
	[[nodiscard]] constexpr inline float radians(const uint32 degrees) noexcept {
		return (degrees >= 0 && degrees < 360) ? angles[degrees] : 0.0f;
		// Invalid index fallback
	}


	[[nodiscard]] constexpr inline float sin(const uint32 sin) noexcept {
		return (sin >= 0 && sin < 360) ? sin_angles[sin] : 0.0f;
		// Invalid index fallback
	}


	[[nodiscard]] constexpr inline float cos(const uint32 cos) noexcept {
		return (cos >= 0 && cos < 360) ? cos_angles[cos] : 0.0f;
		// Invalid index fallback
	}

	// Returns true if `a` can be considered equal to `b` with optional tolerance
	[[nodiscard]] constexpr inline bool compare_floats(const float a, const float b, const float tolerance = 0.01f) {
		return std::abs(a - b) < tolerance;
	}

	// Safe comparison for near-zero floating-point values
	template <typename T>
	[[nodiscard]] constexpr bool is_near_zero(const T value) noexcept {
		return std::fabs(value) < EPSILON;
	}


	// VECTORS //

	// Calculate horizontal orbit (around z-axis) around a center position
	[[nodiscard]] inline vec3<float> orbitate_z(const vec3<float> center_position, const float velocity, const float radius = 5.0f) {
		float angle = ScarabMath::radians(velocity);
		return center_position + vec3<float>(radius * std::cos(angle), 0.0f, radius * std::sin(angle));
	}

	// Calculate vertical orbit (around x-axis) around a center position
	[[nodiscard]] inline vec3<float> orbitate_x(const vec3<float> center_position, const float velocity, const float radius = 5.0f) {
		float angle = ScarabMath::radians(velocity);
		return center_position + vec3<float>(0.0f, radius * std::cos(angle), radius * std::sin(angle));
	}

	// Calculate orbit around the y-axis (xz-plane)
	[[nodiscard]] inline vec3<float> orbitate_y(const vec3<float> center_position, const float velocity, const float radius = 5.0f) {
		float angle = ScarabMath::radians(velocity);
		return center_position + vec3<float>(radius * std::cos(angle), radius * std::sin(angle), radius * std::sin(angle));
	}

	// Return the distance between two positions
	[[nodiscard]] inline float distance_between(const vec3<float>& pos1, const vec3<float> pos2) {
		return glm::length(pos1) - glm::length(pos2);
	}
}
