#pragma once

#include <cmath> // std::sqrt

template <typename T>
struct vec5 {
	T x = 0;
	T y = 0;
	T z = 0;
	T w = 0;
	T h = 0;

	// Constructors
	vec5() = default;
	vec5(T num) : x(num), y(num), z(num), w(num), h(num) {}
	vec5(T x, T y, T z, T w, T h) : x(x), y(y), z(z), w(w), h(h) {}
	vec5(const vec5& other) = default;

	// Move constructor
	vec5(vec5&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)), z(std::move(other.z)), w(std::move(other.w)), h(std::move(other.h)) {
		other.clear();
	}


	// Set all values to zero
	inline void clear() noexcept {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
		this->h = 0;
	}

	// Set values of vector
	inline void set_values(T x, T y, T z, T w, T h) noexcept {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->h = h;
	}


	// Multiplies and sums values with another vector
	constexpr T dot(const vec5<T>& other) const noexcept {
		return (x * other.x + y * other.y + z * other.z + w * other.w + h * other.h);
	}

	// The magnitude (length) of the vector
	constexpr T length() const noexcept {
		return std::sqrt(this->length_squared());
	}

	// The squared magnitude of the vector
	constexpr T length_squared() const noexcept {
		return this->dot(*this);
	}

	// Normalizes the vector to have a magnitude of 1 (unit vector)
	constexpr vec5 normalize() const noexcept {
		const T len = this->length();
		return (len > 0) ? *this / len : *this;
	}


	// Addition
	constexpr vec5 operator+(const vec5<T>& other) const noexcept {
		return vec5(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w, this->h + other.h);
	}

	constexpr vec5 operator+(T scalar) const noexcept {
		return vec5(this->x + scalar, this->y + scalar, this->z + scalar, this->w + scalar, this->h + scalar);
	}

	constexpr vec5& operator+=(const vec5& other) noexcept {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;
		this->h += other.h;
		return *this;
	}

	constexpr vec5& operator+=(T scalar) noexcept {
		this->x += scalar;
		this->y += scalar;
		this->z += scalar;
		this->w += scalar;
		this->h += scalar;
		return *this;
	}


	// Subtraction
	constexpr vec5 operator-(const vec5<T>& other) const noexcept {
		return vec5(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w, this->h - other.h);
	}

	constexpr vec5 operator-(T scalar) const noexcept {
		return vec5(this->x - scalar, this->y - scalar, this->z - scalar, this->w - scalar, this->h - scalar);

	}

	constexpr vec5& operator-=(const vec5& other) noexcept {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;
		this->h -= other.h;
		return *this;
	}

	constexpr vec5& operator-=(T scalar) noexcept {
		this->x -= scalar;
		this->y -= scalar;
		this->z -= scalar;
		this->w -= scalar;
		this->h -= scalar;
		return *this;
	}


	// Multiplication
	constexpr vec5 operator*(const vec5<T>& other) const noexcept {
		return vec5(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w, this->h * other.h);
	}

	constexpr vec5 operator*(T scalar) const noexcept {
		return vec5(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar, this->h * scalar);
	}

	constexpr vec5& operator*=(const vec5& other) noexcept {
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		this->w *= other.w;
		this->h *= other.h;
		return *this;
	}

	constexpr vec5& operator*=(T scalar) noexcept {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		this->h *= scalar;
		return *this;
	}


	// Division
	constexpr vec5 operator/(const vec5<T>& other) const noexcept {
		assert(other.x != 0 && "Division by zero in x component");
		assert(other.y != 0 && "Division by zero in y component");
		assert(other.z != 0 && "Division by zero in z component");
		assert(other.w != 0 && "Division by zero in w component");
		assert(other.h != 0 && "Division by zero in v component");
		return vec5(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w, this->h / other.h);
	}

	constexpr vec5 operator/(T scalar) const noexcept {
		assert(scalar != 0 && "Division by zero");
		return vec5(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar, this->h / scalar);
	}

	constexpr vec5& operator/=(const vec5& other) noexcept {
		assert(other.x != 0 && "Division by zero in x component");
		assert(other.y != 0 && "Division by zero in y component");
		assert(other.z != 0 && "Division by zero in z component");
		assert(other.w != 0 && "Division by zero in w component");
		assert(other.h != 0 && "Division by zero in v component");
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		this->w /= other.w;
		this->h /= other.h;
		return *this;
	}

	constexpr vec5& operator/=(T scalar) noexcept {
		assert(scalar != 0 && "Division by zero");
		this->x /= scalar;
		this->y /= scalar;
		this->z /= scalar;
		this->w /= scalar;
		this->h /= scalar;
		return *this;
	}


	// Equality
	bool operator==(const vec5<T>& other) const noexcept {
		return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w && this->v == other.h);
	}

	bool operator!=(const vec5<T>& other) const noexcept {
		return !(*this == other);
	}


	// Assignment
	vec5<T>& operator=(const vec5<T>& other) noexcept {
		if (this != &other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			this->h = other.h;
		}
		return *this;
	}

	vec5<T>& operator=(T scalar) noexcept {
		this->x = scalar;
		this->y = scalar;
		this->z = scalar;
		this->w = scalar;
		this->h = scalar;
		return *this;
	}

	// Move assignment operator
	vec5<T>& operator=(vec5<T>&& other) noexcept {
		if (this != &other) {
			this->x = std::move(other.x);
			this->y = std::move(other.y);
			this->z = std::move(other.z);
			this->w = std::move(other.w);
			this->h = std::move(other.h);
			other.clear();
		}
		return *this;
	}
};

