#pragma once

#include <cmath>

// Helper namespace with methods related to string manipulation
namespace MathHelper {
	constexpr float EPSILON = 1e-6f; // Small tolerance value for floating-point comparison

	// Convert degrees to radians 
	[[nodiscard]] inline float radians(const float degrees) {
		return degrees * static_cast<float>(0.01745329251994329576923690768489);
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
}
