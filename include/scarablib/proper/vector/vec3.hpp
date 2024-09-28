#pragma once

#include <cmath> // std::sqrt
#include <glm/ext/vector_float3.hpp>

template <typename T>
struct vec3 {
	T x = 0;
	T y = 0;
	T z = 0;

	vec3() = default;
	vec3(T num) : x(num), y(num), z(num) {}
	vec3(T x, T y, T z) : x(x), y(y), z(z) {}
	vec3(const vec3& other) = default; // Copy constructor

	// Move constructor
	vec3(vec3&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)) {
		other.clear();
	}


	// Set all values to zero
	inline void clear() noexcept {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	// Set values of vector
	inline void set_values(T x, T y, T z) noexcept {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// Converts to glm equivalent
	inline glm::vec3 to_glm() const {
		return glm::vec3(this->x, this->y, this->z);
	}


	// 3D Cross product
	constexpr vec3 cross(const vec3<T>& other) const noexcept {
		return vec3(
			this->y * other.z - this->z * other.y,
			this->z * other.x - this->x * other.z,
			this->x * other.y - this->y * other.x
		);
	}

	// Multiplies and sums values with another vector
	constexpr T dot(const vec3<T>& other) const noexcept {
		return (this->x * other.x + this->y * other.y + this->z * other.z);
	}

	// The squared magnitude of the vector
	constexpr T length_squared() const noexcept {
		return this->dot(*this);
	}

	// The magnitude (length) of the vector
	constexpr T length() const noexcept {
		return std::sqrt(this->length_squared());
	}

	// Normalizes the vector to have a magnitude of 1 (unit vector)
	constexpr vec3 normalize() const noexcept {
		const T length = this->length();
		return (length > 0) ? *this / length : *this;
	}


	// Addition
	constexpr vec3 operator+(const vec3<T>& other) const noexcept {
		return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
	}

	constexpr vec3 operator+(const T scalar) const noexcept {
		return vec3(this->x + scalar, this->y + scalar, this->z + scalar);
	}

	constexpr vec3& operator+=(const vec3& other) noexcept {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	constexpr vec3& operator+=(const T scalar) noexcept {
		this->x += scalar;
		this->y += scalar;
		this->z += scalar;
		return *this;
	}


	// Subtraction
	constexpr vec3 operator-(const vec3<T>& other) const noexcept {
		return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	constexpr vec3 operator-(const T scalar) const noexcept {
		return vec3(this->x - scalar, this->y - scalar, this->z - scalar);
	}

	constexpr vec3& operator-=(const vec3& other) noexcept {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	constexpr vec3& operator-=(const T scalar) noexcept {
		this->x -= scalar;
		this->y -= scalar;
		this->z -= scalar;
		return *this;
	}


	// Multiplication
	constexpr vec3 operator*(const vec3<T>& other) const noexcept {
		return vec3(this->x * other.x, this->y * other.y, this->z * other.z);
	}

	constexpr vec3 operator*(const T scalar) const noexcept {
		return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
	}

	constexpr vec3& operator*=(const vec3& other) noexcept {
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		return *this;
	}

	constexpr vec3& operator*=(const T scalar) noexcept {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}


	// Division
	constexpr vec3 operator/(const vec3<T>& other) const noexcept {
		assert(other.x != 0 && "Division by zero in x component");
		assert(other.y != 0 && "Division by zero in y component");
		assert(other.z != 0 && "Division by zero in z component");
		return vec3(this->x / other.x, this->y / other.y, this->z / other.z);
	}

	constexpr vec3 operator/(const T scalar) const noexcept {
		assert(scalar != 0 && "Division by zero");
		return vec3(this->x / scalar, this->y / scalar, this->z / scalar);
	}

	constexpr vec3& operator/=(const vec3& other) noexcept {
		assert(other.x != 0 && "Division by zero in x component");
		assert(other.y != 0 && "Division by zero in y component");
		assert(other.z != 0 && "Division by zero in z component");
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		return *this;
	}

	constexpr vec3& operator/=(const T scalar) noexcept {
		assert(scalar != 0 && "Division by zero");
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;
		return *this;
	}


	// Equality
	bool operator==(const vec3<T>& other) const noexcept {
		return (this->x == other.x && this->y == other.y && this->z == other.z);
	}

	bool operator!=(const vec3<T>& other) const noexcept {
		return !(*this == other);
	}


	// Assignment
	vec3<T>& operator=(const vec3<T>& other) noexcept {
		if(this != &other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		return *this;
	}

	vec3<T>& operator=(const T other) noexcept {
		this->x = other;
		this->y = other;
		this->z = other;

		return *this;
	}

	// Move assignment operator
	vec3<T>& operator=(vec3<T>&& other) noexcept {
		if (this != &other) {
			this->x = std::move(other.x);
			this->y = std::move(other.y);
			this->z = std::move(other.z);
			other.clear();
		}
		return *this;
	}
};

