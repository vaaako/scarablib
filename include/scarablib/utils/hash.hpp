#pragma once

#include <functional>

namespace ScarabHash {
	// From boost library.
	// To multiple values, keep passing the same variable as `seed` until you done.
	// Is VERY recommended to initialize `seed` before using it.
	// Do NOT use raw pointers like `char*` or `const char*`, it will not work.
	// Converting to `std::string` is fine though
	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& value) noexcept {
		// Use golden ration to mix the hash
		std::hash<T> h;
		seed ^= h(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	// Makes a hash out of a value and return it.
	// Do NOT use raw pointers like `char*` or `const char*`, it will nor work
	template <typename T>
	size_t hash_make(const T& value) noexcept {
		size_t seed = 0;
		std::hash<T> h;
		seed ^= h(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}

	// Combine two hashes into a single one
	inline constexpr size_t combine_hashes(size_t a, size_t b) noexcept {
		return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
	}
};
