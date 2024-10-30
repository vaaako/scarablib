#include "scarablib/proper/vector/vec3.hpp"
#include "scarablib/proper/vector/vec4.hpp"
#include "scarablib/utils/math.hpp"

namespace vecutil {
	// VEC4

	// 4D Cross product
	template <typename T>
	[[nodiscard]] constexpr vec4<T> cross(const vec4<T>& a, const vec4<T>& b)  noexcept {
		return vec4(
			(a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x),
			T(0) // 4D cross product is often defined in 3D subspace, so w-component is set to 0
		);
	}

	// Multiplies and sums values with another vector
	template <typename T>
	[[nodiscard]] constexpr T dot(const vec4<T>& a, const vec4<T>& b)  noexcept {
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
	}

	// The magnitude (length) of the vector
	template <typename T>
	[[nodiscard]] constexpr T magnitude(const vec4<T>& vector)  noexcept {
		return std::sqrt(vecutil::dot(vector,vector));
	}

	// The length (magnitude) of the vector
	template <typename T>
	[[nodiscard]] constexpr T length(const vec4<T>& vector)  noexcept {
		return std::sqrt(vecutil::dot(vector, vector));
	}

	// Normalizes the vector to have a magnitude of 1 (unit vector)
	template <typename T>
	[[nodiscard]] constexpr vec4<T> normalized(const vec4<T>& vector)  noexcept {
		const T length = vecutil::magnitude(vector);
		if(!MathHelper::is_near_zero(length)) {
			return vector / length;
		}
		return vector;
	}



	// VEC3

	// 3D Cross product
	template <typename T>
	[[nodiscard]] constexpr vec3<T> cross(const vec3<T>& a, const vec3<T>& b) noexcept {
		return vec3(
			(a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x)
		);
	}

	// Multiplies and sums values with another vector
	template <typename T>
	[[nodiscard]] constexpr T dot(const vec3<T>& a, const vec3<T>& b) noexcept {
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	// The magnitude (length) of the vector
	template <typename T>
	[[nodiscard]] constexpr T magnitude(const vec3<T>& vector)noexcept {
		return std::sqrt(vecutil::dot(vector, vector));
	}

	// The length (magnitude) of the vector
	template <typename T>
	[[nodiscard]] constexpr T length(const vec3<T>& vector)noexcept {
		return std::sqrt(vecutil::dot(vector, vector));
	}

	// Normalizes the vector to have a magnitude of 1 (unit vector)
	template <typename T>
	[[nodiscard]] constexpr vec3<T> normalize(const vec3<T>& vector) noexcept {
		const T length = vecutil::magnitude(vector);
		if(!MathHelper::is_near_zero(length)) {
			return vector / length;
		}
		return vector;
	}


	// Calculate horizontal orbit (around z-axis) around a center position
	[[nodiscard]] inline vec3<float> orbitate_z(const vec3<float> center_position, const float velocity, const float radius = 5.0f) {
		float angle = MathHelper::radians(velocity);
		return center_position + vec3<float>(radius * std::cos(angle), 0.0f, radius * std::sin(angle));
	}

	// Calculate vertical orbit (around x-axis) around a center position
	[[nodiscard]] inline vec3<float> orbitate_x(const vec3<float> center_position, const float velocity, const float radius = 5.0f) {
		float angle = MathHelper::radians(velocity);
		return center_position + vec3<float>(0.0f, radius * std::cos(angle), radius * std::sin(angle));
	}

	// Calculate orbit around the y-axis (xz-plane)
	[[nodiscard]] inline vec3<float> orbitate_y(const vec3<float> center_position, const float velocity, const float radius = 5.0f) {
		float angle = MathHelper::radians(velocity);
		return center_position + vec3<float>(radius * std::cos(angle), radius * std::sin(angle), radius * std::sin(angle));
	}

	// Return the distance between two positions
	[[nodiscard]] inline float distance_between(const vec3<float>& pos1, const vec3<float> pos2) {
		return vecutil::magnitude(pos1) - vecutil::magnitude(pos2);
	}
}
