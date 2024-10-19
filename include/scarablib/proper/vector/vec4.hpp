#pragma once

#include <cmath> // std::sqrt
#include <glm/ext/vector_float4.hpp>

template <typename T>
struct vec4 {
	T x = 0;
	T y = 0;
	T z = 0;
	T w = 0;

	// Constructors
	vec4() = default;
	vec4(T num) : x(num), y(num), z(num), w(num) {}
	vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
	vec4(const vec4& other) = default; // Copy constructor

	// Move constructor
	vec4(vec4&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)), w(std::move(other.w)) {
		other.clear();
	}


	// Set all values to zero
	inline void clear() noexcept {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	// Set values of vector
	inline void set_values(T x, T y, T z, T w) noexcept {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	// Converts to glm equivalent
	inline glm::vec4 to_glm() const {
		return glm::vec4(this->x, this->y, this->z, this->w);
	}


	// 4D Cross product
	constexpr vec4 cross(const vec4<T>& other) const noexcept {
		return vec4(
			this->y * other.z - this->z * other.y,
			this->z * other.x - this->x * other.z,
			this->x * other.y - this->y * other.x,
			T(0) // 4D cross product is often defined in 3D subspace, so w-component is set to 0
		);
	}

	// Multiplies and sums values with another vector
	constexpr T dot(const vec4<T>& other) const noexcept {
		return (this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w);
	}

	// The magnitude (length) of the vector
	constexpr T magnitude() const noexcept {
		return std::sqrt(this->dot(*this));
	}

	// Normalizes the vector to have a magnitude of 1 (unit vector)
	constexpr vec4 normalized() const noexcept {
		const T length = this->magnitude();
		return (length > 0) ? *this / length : *this;
	}


	// Addition
	constexpr vec4 operator+(const vec4<T>& other) const noexcept {
		return vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
	}

	constexpr vec4 operator+(const T scalar) const noexcept {
		return vec4(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar);
	}

	constexpr vec4& operator+=(const vec4& other) noexcept {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;
		return *this;
	}

	constexpr vec4& operator+=(const T scalar) noexcept {
		this->x += scalar;
		this->y += scalar;
		this->z += scalar;
		this->w += scalar;
		return *this;
	}


	// Subtraction
	constexpr vec4 operator-(const vec4<T>& other) const noexcept {
		return vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
	}

	constexpr vec4 operator-(const T scalar) const noexcept {
		return vec4(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar);
	}

	constexpr vec4& operator-=(const vec4& other) noexcept {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;
		return *this;
	}

	constexpr vec4& operator-=(const T scalar) noexcept {
		this->x -= scalar;
		this->y -= scalar;
		this->z -= scalar;
		this->w -= scalar;
		return *this;
	}


	// Multiplication
	constexpr vec4 operator*(const vec4<T>& other) const noexcept {
		return vec4(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
	}

	constexpr vec4 operator*(const T scalar) const noexcept {
		return vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
	}

	constexpr vec4& operator*=(const vec4& other) noexcept {
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		this->w *= other.w;
		return *this;
	}

	constexpr vec4& operator*=(const T scalar) noexcept {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}


	// Division
	constexpr vec4 operator/(const vec4<T>& other) const noexcept {
		assert(other.x != 0 && "Division by zero in x component");
		assert(other.y != 0 && "Division by zero in y component");
		assert(other.z != 0 && "Division by zero in z component");
		assert(other.w != 0 && "Division by zero in w component");
		return vec4(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w);
	}

	constexpr vec4 operator/(const T scalar) const noexcept {
		assert(scalar != 0 && "Division by zero");
		return vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar);
	}

	constexpr vec4& operator/=(const vec4& other) noexcept {
		assert(other.x != 0 && "Division by zero in x component");
		assert(other.y != 0 && "Division by zero in y component");
		assert(other.z != 0 && "Division by zero in z component");
		assert(other.w != 0 && "Division by zero in w component");
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		this->w /= other.w;
		return *this;
	}

	constexpr vec4& operator/=(const T scalar) noexcept {
		assert(scalar != 0 && "Division by zero");
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;
		this->w /= scalar;
		return *this;
	}


	// Equality
	bool operator==(const vec4<T>& other) const noexcept {
		return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
	}

	bool operator!=(const vec4<T>& other) const noexcept {
		return !(*this == other);
	}


	// Assignment
	vec4<T>& operator=(const vec4<T>& other) noexcept {
		if (this != &other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
		}
		return *this;
	}

	vec4<T>& operator=(const T& other) noexcept {
		this->x = other;
		this->y = other;
		this->z = other;
		this->w = other;
		return *this;
	}

	// Move assignment
	constexpr vec4& operator=(vec4&& other) noexcept {
		if (this != &other) {
			this->x = std::move(other.x);
			this->y = std::move(other.y);
			this->z = std::move(other.z);
			this->w = std::move(other.w);
			other.clear();
		}
		return *this;
	}
};

