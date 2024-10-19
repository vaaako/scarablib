#pragma once

#include <cmath> // std::sqrt
#include <glm/ext/vector_float2.hpp>

template <typename T>
struct vec2 {
	T x = 0;
	T y = 0;

	// The constructors NEED to be on header file
	vec2() = default;
	vec2(T num) : x(num), y(num) {}
	vec2(T x, T y) : x(x), y(y) {}
	vec2(const vec2& other) = default; // Copy constructor

	// Move constructor
	vec2(vec2&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {
		other.clear();
	}


	// Set all values to zero
	inline void clear() noexcept {
		this->x = 0;
		this->y = 0;
	}

	// Set values of vector
	inline void set_values(T x, T y) noexcept {
		this->x = x;
		this->y = y;
	}

	// Converts to glm equivalent
	inline glm::vec2 to_glm() const {
		return glm::vec2(this->x, this->y);
	}


	// 2D Cross product (returns a scalar)
	constexpr T cross(const vec2<T>& other) const noexcept {
		return this->x * other.y - this->y * other.x;
	}

	// Multiplies and sums values with another vector
	constexpr T dot(const vec2& other) const noexcept {
		return this->x * other.x + this->y * other.y;
	}

	// The magnitude (length) of the vector
	constexpr T magnitude() const noexcept {
		return std::sqrt(this->dot(*this));
	}

	// Normalizes the vector to have a magnitude of 1 (unit vector)
	constexpr vec2 normalized() const noexcept {
		const T length = this->magnitude();
		return (length > 0) ? *this / length : *this;
	}


	// Addition
	constexpr vec2 operator+(const vec2<T>& other) const noexcept {
		return vec2(this->x + other.x, this->y + other.y);
	}

	constexpr vec2 operator+(const T scalar) const noexcept {
		return vec2(this->x + scalar, this->y + scalar);
	}

	constexpr vec2& operator+=(const vec2& other) noexcept {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	constexpr vec2& operator+=(const T scalar) noexcept {
		this->x += scalar;
		this->y += scalar;
		return *this;
	}


	// Subtraction
	constexpr vec2 operator-(const vec2<T>& other) const noexcept {
		return vec2(this->x - other.x, this->y - other.y);
	}

	constexpr vec2 operator-(const T scalar) const noexcept {
		return vec2(this->x - scalar, this->y - scalar);
	}

	constexpr vec2& operator-=(const vec2& other) noexcept {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	constexpr vec2& operator-=(const T scalar) noexcept {
		this->x -= scalar;
		this->y -= scalar;
		return *this;
	}


	// Multiplication
	constexpr vec2 operator*(const vec2<T>& other) const noexcept {
		return vec2(this->x * other.x, this->y * other.y);
	}

	constexpr vec2 operator*(const T scalar) const noexcept {
		return vec2(this->x * scalar, this->y * scalar);
	}

	constexpr vec2& operator*=(const vec2& other) noexcept {
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	constexpr vec2& operator*=(const T scalar) noexcept {
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}


	// Division
	constexpr vec2 operator/(const vec2<T>& other) const noexcept {
		assert(other.x != 0 && "Division by zero in x component");
		assert(other.y != 0 && "Division by zero in y component");
		return vec2(this->x / other.x, this->y / other.y);
	}

	constexpr vec2 operator/(const T scalar) const noexcept {
		assert(scalar != 0 && "Division by zero");
		return vec2(this->x / scalar, this->y / scalar);
	}

	constexpr vec2& operator/=(const vec2& other) noexcept {
		this->x /= other.x;
		this->y /= other.y;
		return *this;
	}

	constexpr vec2& operator/=(const T scalar) noexcept {
		this->x /= scalar;
		this->y /= scalar;
		return *this;
	}


	// Equality
	bool operator==(const vec2<T>& other) const noexcept {
		return (this->x == other.x && this->y == other.y);
	}

	bool operator!=(const vec2<T>& other) const noexcept {
		return !(*this == other);
	}


	// Assigment
	vec2<T>& operator=(const vec2<T>& other) noexcept {
		if(this != &other) {
			this->x = other.x;
			this->y = other.y;
		}
		return *this;
	}

	vec2<T>& operator=(const T other) noexcept {
		this->x = other;
		this->y = other;
		return *this;
	}

	vec2<T>& operator=(vec2<T>&& other) noexcept {
		if (this != &other) {
			this->x = std::move(other.x);
			this->y = std::move(other.y);
			other.clear();
		}
		return *this;
	}
};

