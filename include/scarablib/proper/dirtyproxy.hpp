#pragma once

template <typename T>
struct DirtyProxy {
	private:
		T value;
		bool& isdirty;
	public:
		// Reference to value and isdirty (change one)
		DirtyProxy(const T& value, bool& isdirty) noexcept;

		DirtyProxy(const DirtyProxy&) = delete;
		DirtyProxy& operator=(const DirtyProxy&) = delete;

		constexpr bool is_dirty() const noexcept {
			return this->isdirty;
		}

		constexpr bool operator==(const T& rhs) const noexcept {
			return this->value == rhs;
		}

		constexpr DirtyProxy& operator=(const T& new_value) noexcept {
			this->value = new_value;
			this->isdirty = true;
			return *this;
		}

		constexpr T* operator->() noexcept {
			this->isdirty = true; // Assume non-const acess is a modification
			return &this->value;
		}

		// Allow access to members
		constexpr const T* operator->() const noexcept {
			return &this->value;
		}

		// Be able to use this class as the value
		constexpr operator const T&() const noexcept {
			return this->value;
		}


		constexpr DirtyProxy& operator+=(const T& rhs) noexcept {
			this->value += rhs;
			this->isdirty = true;
			return *this;
		}

		constexpr DirtyProxy& operator-=(const T& rhs) noexcept {
			this->value -= rhs;
			this->isdirty = true;
			return *this;
		}

		constexpr DirtyProxy& operator*=(const T& rhs) noexcept {
			this->value *= rhs;
			this->isdirty = true;
			return *this;
		}

		constexpr DirtyProxy& operator/=(const T& rhs) noexcept {
			this->value /= rhs;
			this->isdirty = true;
			return *this;
		}

		template <typename U>
		constexpr T operator+(const U& rhs) const {
			return this->value + rhs;
		}

		template <typename U>
		constexpr T operator-(const U& rhs) const {
			return this->value - rhs;
		}

		template <typename U>
		constexpr T operator*(const U& rhs) const {
			return this->value * rhs;
		}

		template <typename U>
		constexpr T operator/(const U& rhs) const {
			return this->value / rhs;
		}
};

template <typename T>
DirtyProxy<T>::DirtyProxy(const T& value, bool& isdirty) noexcept : value(value), isdirty(isdirty) {}
